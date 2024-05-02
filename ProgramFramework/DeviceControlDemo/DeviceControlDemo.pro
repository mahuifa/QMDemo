#---------------------------------------------------------
# 功能：       当软件需要和多个设备(例如传感器)进行通信时，而每个设备的
#             通信方式又不一样（TCP、UDP、串口），这时就可以使用多态，
#             定义一个通信接口基类，统一管理所有的通信方式，同时定义一个数据
#             解析基类，可支持创建线程解析数据
# 编译器：     Desktop Qt 5.12.12 MinGW 64-bit（也支持其它编译器）
# Qt IDE：    D:/Qt/Qt5.12.12/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-05-11 10:39:32
# @备注
#---------------------------------------------------------
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    abstractsensor.cpp \
    abstractthread.cpp \
    handletcpthread.cpp \
    main.cpp \
    tcpsensor.cpp \
    widget.cpp

HEADERS += \
    abstractsensor.h \           # 设备通信基类
    abstractthread.h \           # 数据解析基类
    handletcpthread.h \          # Tcp接口通信数据解析线程类
    tcpsensor.h \                # 使用Tcp通信的设备接口类
    widget.h

FORMS += widget.ui

#  定义程序版本号
VERSION = 1.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

# msvc >= 2017  编译器使用utf-8编码
msvc {
    greaterThan(QMAKE_MSC_VER, 1900){       # msvc编译器版本大于2015
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }else{
#        message(msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码)
    }
}
