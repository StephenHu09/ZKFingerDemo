#ifndef ZKFINGERDEV_H
#define ZKFINGERDEV_H

/**
 *	@file		zkfingerdev.h
 *	@brief		指纹仪设备功能接口调用（型号 ZKT-ECO Live20R）
 */

#include <QObject>
#include <QLabel>

#include "zkfinger.h"

#define ENROLLCNT 3

enum FingerMsgType {
    DEV_CONNECT_TYPE = 0,
    DEV_DISCONNECT_TYPE,
    INIT_SUCCESS_TYPE,
    INIT_FAILED_TYPE,
    DB_INIT_SUCCESS_TYPE,
    DB_INIT_FAILED_TYPE,
    OPEN_SUCCESS_TYPE,
    OPEN_FAILED_TYPE,
    REGISTER_SUCCESS_TYPE,
    REGISTER_FAILED_TYPE,
    IDENTIFY_SUCCESS_TYPE,
    IDENTIFY_FAILED_TYPE,

    LOG_MSG_TYPE
};

class ZKFingerDev : public QObject
{
    Q_OBJECT
public:
    explicit ZKFingerDev(QObject *parent = nullptr);
    ~ZKFingerDev();

    void InitDevice();
    void RemoveDevice();
    void SetLabelView(QLabel *labelView);
    void StartModule();

    void StartRegister();
    void StartVerify();
    void StartDbCacheClear();

private:
    void ShowFpImage(unsigned char* pImgBuf, int width, int height);
    void DoRegister(unsigned char* pTemplate, int len);
    void DoVerify(unsigned char* pTemplate, int len);


signals:
    void sigMessage(int type, const QString &msg);

private slots:
    void onCapture();


private:
    QSharedPointer<ZkFinger> m_zkfinger;  // lib库接口实例
    void* m_hDevice;   // 设备句柄

    int m_imgFPWidth;
    int m_imgFPHeight;
    QLabel *m_fpView;  // 显示图像
    unsigned char* m_pImgBuf; // 图像数据保存

    int m_nFakeFunOn; // 是否开启fake finger判断
    bool m_bIdentify; // 指纹验证标志位
    bool m_bRegister; // 指纹登记标志位

    QStringList m_preRegTempList;
    int m_nLastRegTempLen;
    QString m_szLastRegTemplate;

    int m_Tid; // 指纹登记id
};

#endif // ZKFINGERDEV_H
