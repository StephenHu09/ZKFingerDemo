#ifndef ZKFINGERDEMO_H
#define ZKFINGERDEMO_H

#include <QObject>
#include <QMainWindow>
#include <QLabel>

class ZKFingerDev;

QT_BEGIN_NAMESPACE
namespace Ui { class ZkFingerDemo; }
QT_END_NAMESPACE

class ZkFingerDemo : public QMainWindow
{
    Q_OBJECT

public:
    ZkFingerDemo(QWidget *parent = nullptr);
    ~ZkFingerDemo();

private slots:
    void HandleMessage(int type, const QString &msg);

    void on_btnReg_clicked();
    void on_btnVerify_clicked();
    void on_btnClear_clicked();
    void on_btnConnect_clicked();

private:
    Ui::ZkFingerDemo *ui;
    QSharedPointer<ZKFingerDev> m_zkfDev;
};
#endif // ZKFINGERDEMO_H
