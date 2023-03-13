QT += core gui
QT += sql
QT += websockets

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

win32 {
    LIBS += -L$$PWD/libs/win/x86lib
    INCLUDEPATH += $$PWD/libs/win/include/
}

win64 {
    LIBS += -L$$PWD/libs/win/x64lib
    INCLUDEPATH += $$PWD/libs/win/include/
    #LIBS += -L$$PWD/libs/win/x64lib -llibzkfp
}

unix {
    LIBS += -L$$PWD/libs/linux
    INCLUDEPATH += $$PWD/libs/linux/include/
}

win* {
HEADERS += \
    $$PWD/libs/win/include/libzkfp.h \
    $$PWD/libs/win/include/libzkfperrdef.h \
    $$PWD/libs/win/include/libzkfptype.h \
    $$PWD/libs/win/include/zkinterface.h
}

unix {
HEADERS += \
    $$PWD/libs/linux/include/libzkfp.h \
    $$PWD/libs/linux/include/libzkfperrdef.h \
    $$PWD/libs/linux/include/libzkfptype.h \
    $$PWD/libs/linux/include/zkinterface.h
}



SOURCES += \
    $$PWD/zkfinger.cpp \
    $$PWD/zkfingerdev.cpp

HEADERS += \
    $$PWD/zkfinger.h \
    $$PWD/zkfingerdev.h


