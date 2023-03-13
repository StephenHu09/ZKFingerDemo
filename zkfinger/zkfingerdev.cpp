#include "zkfingerdev.h"

#include <QDebug>
#include <QTimer>


// QString 转 unsigned char 数组
QByteArray QStringToChar(const QString& str)
{
    /* 返回值可通过如下方式使用： reinterpret_cast<unsigned char*>(byteArray.data()) */
    QByteArray byteArray = QByteArray::fromBase64(str.toLatin1().data());
    return byteArray;
}

// unsigned char 数组转 QString
QString CharToQString(const unsigned char* buffer, int length)
{
    QByteArray ba(reinterpret_cast<const char*>(buffer), length);
    QString str = ba.toBase64();
    return str;
}

ZKFingerDev::ZKFingerDev(QObject *parent) : QObject(parent)
{
    m_zkfinger.reset(new ZkFinger());
    m_hDevice = nullptr;
    m_fpView = nullptr;
}

ZKFingerDev::~ZKFingerDev()
{
    qDebug() << "========== free ZKFingerDev ==========";
    RemoveDevice();
    m_fpView = nullptr;
}

void ZKFingerDev::InitDevice()
{
    if (m_zkfinger.isNull()) {
        qCritical() << "======== m_zkfinger is nullptr";
        return;
    }

    if (m_hDevice != nullptr) {
        qInfo() << "finger device already connect";
        return;
    }

    int ret = m_zkfinger->InitSensor();
    if (ZKFP_ERR_OK != ret) {
        qDebug() << "sensor init failed, ret =" << ret;
        emit sigMessage(INIT_FAILED_TYPE, QString("指纹仪初始化失败，请检查设备"));
        return;
    }
    emit sigMessage(INIT_SUCCESS_TYPE, "");

    m_hDevice = m_zkfinger->OpenDevice(0);
    if (m_hDevice == nullptr) {
        m_zkfinger->DeInitSensor();
        qDebug() << "OpenDevice failed";
        emit sigMessage(OPEN_FAILED_TYPE, QString("指纹仪打开失败，请检查设备"));
        return;
    }
    qDebug() << ">>>>> OpenDevice success <<<<<";
    emit sigMessage(OPEN_SUCCESS_TYPE, "");

    // int count = m_zkfinger->GetDeviceCount();
    // qDebug() << "zk finger dev count =" << count;

    if (!m_zkfinger->DBInit()) {
        m_zkfinger->CloseDevice(m_hDevice);
        m_zkfinger->DeInitSensor();
        m_hDevice = nullptr;
        qDebug() << "DBInit failed";
        emit sigMessage(DB_INIT_FAILED_TYPE, QString("指纹仪内部数据模块初始化失败，请检查设备"));
        return;
    }
    qInfo() << ">>>>> ZkFinger DBInit success <<<<<";
    emit sigMessage(DB_INIT_SUCCESS_TYPE, "");

    /**
     * // 设置图像 DPI(儿童建议设置 750/1000), 可选
     * int nDPI = 750;
     * ZKFPM_SetParameters(m_hDevice, 3, (unsigned char*)&nDPI, sizeof(int));
     */

    // set FakeFun On
    m_nFakeFunOn = 1;
    m_zkfinger->SetParameters(m_hDevice, 2002, (unsigned char*)&m_nFakeFunOn, sizeof(int));

    unsigned int size = 4;
    m_zkfinger->GetParameters(m_hDevice, 1, (unsigned char*)&m_imgFPWidth, &size);
    size = 4;
    m_zkfinger->GetParameters(m_hDevice, 2, (unsigned char*)&m_imgFPHeight, &size);
    qDebug() << "Get m_imgFPWidth =" << m_imgFPWidth << ", m_imgFPHeight =" << m_imgFPHeight;

    /** // 方式2，获取指纹图像的长宽
     * TZKFPCapParams zkfpCapParams = {0x0};
     * m_zkfinger->GetCaptureParams(m_hDevice, &zkfpCapParams);
     * qDebug() << "GetCapture imgWidth =" << zkfpCapParams.imgWidth
     *          << ", imgHeight =" << zkfpCapParams.imgHeight
     *          << ", nDPI =" << zkfpCapParams.nDPI;
     */

    m_pImgBuf = new unsigned char[m_imgFPWidth * m_imgFPHeight];

    m_Tid = 1;
    m_preRegTempList.clear();
    m_bRegister = false;
    m_bIdentify = true; // 默认开启验证模式
    m_nLastRegTempLen = 0;

    qDebug() << "========= Init Device finish =========";
    emit sigMessage(DEV_CONNECT_TYPE, QString("设备连接成功"));
}

void ZKFingerDev::RemoveDevice()
{
    if (m_zkfinger.isNull()) {
        qCritical() << "======== m_zkfinger is nullptr";
        return;
    }

    if (m_hDevice == nullptr) {
        return;
    }

    if (m_pImgBuf != nullptr) {
        delete [] m_pImgBuf;
        m_pImgBuf = nullptr;
    }

    m_zkfinger->DBFree();
    m_zkfinger->CloseDevice(m_hDevice);
    m_zkfinger->DeInitSensor();
    m_hDevice = nullptr;
    m_Tid = 1;

    qDebug() << "Device disconnect ok";
}

void ZKFingerDev::SetLabelView(QLabel *labelView)
{
    m_fpView = labelView;

    if (m_fpView != nullptr) {
        m_fpView->setStyleSheet("border: 1px solid black");
    }
}

void ZKFingerDev::StartModule()
{
    QTimer *captrueTimer = new QTimer(this);
    captrueTimer->setInterval(100);
    connect(captrueTimer, &QTimer::timeout, this, &ZKFingerDev::onCapture);
    captrueTimer->start();
}

void ZKFingerDev::StartRegister()
{
    if (nullptr != m_hDevice) {
        if (!m_bRegister) {
            m_bRegister = true;
            m_preRegTempList.clear();
            QString log = QString("Register 开始指纹登记, 请按压手指3次");
            qCritical() << log;
            emit sigMessage(LOG_MSG_TYPE, log);
        }
    }
}

void ZKFingerDev::StartVerify()
{
    if (nullptr != m_hDevice) {
        if (m_bRegister) {
            m_bRegister = false;
        }
        m_bIdentify = true;
        QString log = QString("Identify 开始指纹验证");
        qCritical() << log;
        emit sigMessage(LOG_MSG_TYPE, log);
    }
}

void ZKFingerDev::StartDbCacheClear()
{
    if (nullptr != m_hDevice) {
        m_zkfinger->DBClear();
        m_nLastRegTempLen = 0;
        m_Tid = 1;
        qDebug() << "IN-Memory DB Clear OK";
        emit sigMessage(LOG_MSG_TYPE, QString("内部存储指纹信息已清除"));
    }
}

void ZKFingerDev::ShowFpImage(unsigned char *pImgBuf, int width, int height)
{
    QImage *fpImg = new QImage(pImgBuf, width, height, QImage::Format_Indexed8);
    QVector<QRgb> grayTable;

    for (int i = 0; i < 256; i++) {
        grayTable.push_back(qRgb(i,i,i));
    }

    fpImg->setColorTable(grayTable);
    if (m_fpView != nullptr) {
        QPixmap pixMap = QPixmap::fromImage(*fpImg);
        // qDebug() << "FpImage: width = " << pixMap.width() << ", height = " << pixMap.height();
        QPixmap fitpixmap = pixMap.scaled(m_fpView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 适配view大小 按比例缩放
        m_fpView->setPixmap(fitpixmap);
    } else {
        qWarning() << "fingerIdViewLabel is not set";
    }

    if (fpImg != nullptr) {
        delete fpImg;
        fpImg = nullptr;
    }

    return;
}

void ZKFingerDev::DoRegister(unsigned char *pTemplate, int len)
{
    if (m_zkfinger.isNull()) {
        return;
    }

    unsigned int fingerId = 0;
    unsigned int score = 0;
    int ret = m_zkfinger->DBIdentify(pTemplate, len, &fingerId, &score);
    if (ZKFP_ERR_OK == ret) {
        qDebug() << QString("错误，当前手指已登记! fingerId=%1, score=%2").arg(fingerId).arg(score);
        emit sigMessage(LOG_MSG_TYPE, QString("错误，当前手指已登记!"));
        m_preRegTempList.clear();
        m_bRegister = false;
        return;
    }

    // 开始指纹注册: 需要同一手指连续验证3次, 指纹特征码临时存储在 m_preRegTempList
    if (m_preRegTempList.size() > 0) {
        QByteArray curTemp = QStringToChar(m_preRegTempList.last());
        if (m_zkfinger->DBMatch(reinterpret_cast<unsigned char*>(curTemp.data()),
                                curTemp.length(), pTemplate, len) <= 0) {
            // 匹配失败, 非同一手指录入
            qDebug() << "Please press the same finger while registering";
            emit sigMessage(LOG_MSG_TYPE, QString("错误，请按压同一个手指"));
            m_preRegTempList.clear();
            m_bRegister = false;
            return;
        }
    }

    m_preRegTempList.append(CharToQString(pTemplate, len));

    if (m_preRegTempList.size() >= ENROLLCNT) {
        unsigned char szRegTemp[MAX_TEMPLATE_SIZE] = {0x0};
        unsigned int cbRegTemp = MAX_TEMPLATE_SIZE;
        QByteArray temp1 = QStringToChar(m_preRegTempList.at(0));
        QByteArray temp2 = QStringToChar(m_preRegTempList.at(1));
        QByteArray temp3 = QStringToChar(m_preRegTempList.at(2));
        ret = m_zkfinger->DBMerge(reinterpret_cast<unsigned char*>(temp1.data()),
                                  reinterpret_cast<unsigned char*>(temp2.data()),
                                  reinterpret_cast<unsigned char*>(temp3.data()),
                                  szRegTemp, &cbRegTemp);

        m_bRegister = false;
        if (ZKFP_ERR_OK != ret) {
            QString log = QString("指纹登记失败: 模板合并失败 ret=%1").arg(ret);
            qDebug() << log;
            emit sigMessage(LOG_MSG_TYPE, log);
            return;
        }

        ret = m_zkfinger->DBAdd(m_Tid++, szRegTemp, cbRegTemp);
        if (ZKFP_ERR_OK != ret) {
            QString log = QString("指纹登记失败: 数据添加失败 ret=%1").arg(ret);
            qDebug() << log;
            emit sigMessage(LOG_MSG_TYPE, log);
            return;
        }

        m_szLastRegTemplate = CharToQString(szRegTemp, cbRegTemp);
        m_nLastRegTempLen = m_szLastRegTemplate.length();
        qDebug() << "Register success";
        emit sigMessage(LOG_MSG_TYPE, QString("指纹登记成功!"));
        emit sigMessage(REGISTER_SUCCESS_TYPE, "");
    } else {
        QString log = QString("您还需要按压手指%1次").arg(ENROLLCNT - m_preRegTempList.size());
        qDebug() << log;
        emit sigMessage(LOG_MSG_TYPE, log);
    }
}

void ZKFingerDev::DoVerify(unsigned char *pTemplate, int len)
{
    if (m_zkfinger.isNull()) {
        return;
    }

    if (m_nLastRegTempLen > 0) { // have enroll one more template
        if (m_bIdentify) {
            unsigned int tid = 0;
            unsigned int score = 0;
            int ret = m_zkfinger->DBIdentify(pTemplate, len, &tid, &score);
            if (ZKFP_ERR_OK == ret) {
                QString log = QString("Identify success 指纹验证成功, tid = %1, score = %2").arg(tid).arg(score);
                qInfo() << log;
                emit sigMessage(LOG_MSG_TYPE, log);
                emit sigMessage(IDENTIFY_SUCCESS_TYPE, "");
            } else {
                QString log = QString("Identify fail 指纹未登记, 验证失败 ret =%1").arg(ret);
                qCritical() << log;
                emit sigMessage(LOG_MSG_TYPE, log);
            }
        } else {
            QByteArray temp = QStringToChar(m_preRegTempList.at(0));
            int ret = m_zkfinger->DBMatch(reinterpret_cast<unsigned char*>(temp.data()), m_szLastRegTemplate.length(), pTemplate, len);
            if (ret < ZKFP_ERR_OK) {
                QString log = QString("指纹匹配失败 ret =%1").arg(ret);
                qCritical() << log;
                emit sigMessage(LOG_MSG_TYPE, log);
            } else {
                QString log = QString("指纹匹配成功 score =%1").arg(ret);
                qInfo() << log;
                emit sigMessage(LOG_MSG_TYPE, log);
            }
        }
    } else {
        QString log = QString("您需要先注册一个指纹信息");
        qInfo() << log;
        emit sigMessage(LOG_MSG_TYPE, log);
    }
}

void ZKFingerDev::onCapture()
{
    if (m_zkfinger.isNull()) {
        return;
    }

    if (m_hDevice == nullptr) {
        return;
    }

    unsigned char szTemplate[MAX_TEMPLATE_SIZE];
    unsigned int tempLen = MAX_TEMPLATE_SIZE;
    // 采集指纹图像和模板
    int ret = m_zkfinger->AcquireFingerprint(m_hDevice, m_pImgBuf, m_imgFPWidth * m_imgFPHeight, szTemplate, &tempLen);
    if (ZKFP_ERR_OK == ret) {
        //FakeFinger Test
        if (1 == m_nFakeFunOn) {
            int nFakeStatus = 0;
            unsigned int retLen = sizeof(int);
            if (0 == m_zkfinger->GetParameters(m_hDevice, 2004, (unsigned char*)&nFakeStatus, &retLen)) {
                if ((nFakeStatus & 0x1F) != 0x1F) {
                    QString log = QString("手指检测无效, Is Fake Finger?");
                    qWarning() << log;
                    emit sigMessage(LOG_MSG_TYPE, log);
                    return;
                }
            }
        }

        ShowFpImage(m_pImgBuf, m_imgFPWidth, m_imgFPHeight);
        if(m_bRegister) {
            qInfo() << QString("开始指纹注册");
            DoRegister(szTemplate, tempLen);
        } else {
            qDebug() << QString("开始指纹验证");
            DoVerify(szTemplate, tempLen);
        }
    } else {
        // qDebug() << "AcquireFingerprint failed, ret =" << ret;
    }
}
