#---------------------------------------------------------
# 功能：       Qt网络通信程序Demo
# 编译器：
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/04/07
# @备注
#---------------------------------------------------------
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include($$PWD/SimpleNetWidget/SimpleNetWidget.pri)    # 较为简单的Qt网络通信学习Demo
INCLUDEPATH += $$PWD/SimpleNetWidget
include($$PWD/NetWidget/NetWidget.pri)                # 较为复杂的Qt网络通信学习Demo
INCLUDEPATH += $$PWD/NetWidget
include($$PWD/NetInterface/NetInterface.pri)          # 网络接口管理模块（查询所有网卡信息和IP地址信息）
INCLUDEPATH += $$PWD/NetInterface

#  定义程序版本号
VERSION = 1.0.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

#msvc  编译器使用utf-8编码
msvc {
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
}
