#include "zkfingerdemo.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("gbk");
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec); // 解决汉字乱码问题
#endif

    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz}]%{file}:%{line} - %{message}"); // 在log头添加时间戳

    QApplication a(argc, argv);
    ZkFingerDemo w;
    w.show();
    return a.exec();
}
