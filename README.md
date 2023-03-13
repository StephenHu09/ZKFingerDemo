### 功能

本用例基于ZKT-ECO live20R指纹仪调试（USB接口），实现基本功能。由于官方SDK 的demo中没有qt实现的版本，现由于工作需要，自行调试实现。

官方SDK源码参考：https://www.zkteco.com/cn/search?keywords=SDK

- windows：ZKFinger SDK 5.0.0.32.rar

- linux：ZKFingerReaderSDK Linux.zip



### 开发调试

win10系统下，基于 Qt 5.14.0  MSCV2017 64bit  调试，完成了基本的指纹登记（Register）和验证（Identify）功能。

先安装官方SDK包中的库文件 __setup.exe__ ,  （会自动安装相关的依赖库）

windows下使用 QtCreator 打开 ZkFingerDemo.pro 文件即可。

Linux 待验证



### 代码说明

zkfinger.h 和 zkfinger.cpp 是基于lib库接口的封装，通过 LoadLibrary 的方式，将lib中定义的接口转换为qt下可以调用的实例；

zkfingerdev.h 和 zkfingerdev.cpp 是设备功能的实现，提供功能操作接口。

ZkFingerDemo 是上层UI实现



