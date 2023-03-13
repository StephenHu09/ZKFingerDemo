#include "zkfingerdemo.h"
#include "ui_zkfingerdemo.h"

#include "zkfingerdev.h"

ZkFingerDemo::ZkFingerDemo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ZkFingerDemo)
{
    ui->setupUi(this);

    ui->logInfo->setText(QString("请选择功能，默认指纹验证模式"));

    m_zkfDev.reset(new ZKFingerDev());
    QObject::connect(m_zkfDev.data(), &ZKFingerDev::sigMessage, this, &ZkFingerDemo::HandleMessage);

    m_zkfDev->InitDevice();
    m_zkfDev->SetLabelView(ui->labelView);
    m_zkfDev->StartModule();
}

ZkFingerDemo::~ZkFingerDemo()
{
    delete ui;
}

void ZkFingerDemo::HandleMessage(int type, const QString &msg)
{
    if (type == LOG_MSG_TYPE) {
        ui->logInfo->setText(msg);
    } else if (type == INIT_FAILED_TYPE || type == DB_INIT_FAILED_TYPE || type == OPEN_FAILED_TYPE) {
        ui->btnReg->setEnabled(false);
        ui->btnVerify->setEnabled(false);
        ui->btnClear->setEnabled(false);
        ui->btnConnect->setText(QString("连接设备"));

        ui->logInfo->setText(msg);
    } else if (type == DEV_CONNECT_TYPE) {
        ui->btnReg->setEnabled(true);
        ui->btnVerify->setEnabled(true);
        ui->btnClear->setEnabled(true);
        ui->btnConnect->setText(QString("断开连接"));
        ui->logInfo->setText(QString("请选择功能，默认指纹验证模式"));
    }
}


void ZkFingerDemo::on_btnReg_clicked()
{
    m_zkfDev->StartRegister();
}

void ZkFingerDemo::on_btnVerify_clicked()
{
    m_zkfDev->StartVerify();
}

void ZkFingerDemo::on_btnClear_clicked()
{
    m_zkfDev->StartDbCacheClear();
}

void ZkFingerDemo::on_btnConnect_clicked()
{
    if (ui->btnConnect->text() == QString("连接设备")) {
        m_zkfDev->InitDevice();
    } else if (ui->btnConnect->text() == QString("断开连接")) {
        m_zkfDev->RemoveDevice();

        ui->btnReg->setEnabled(false);
        ui->btnVerify->setEnabled(false);
        ui->btnClear->setEnabled(false);
        ui->btnConnect->setText(QString("连接设备"));
        ui->logInfo->setText(QString("设备已断开连接"));
    }
}
