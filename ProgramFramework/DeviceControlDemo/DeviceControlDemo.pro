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

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
message("64bit")                # 使用64位编译器
DESTDIR = $$PWD/../bin64
}

# msvc  编译器使用utf-8编码
msvc {
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
}
