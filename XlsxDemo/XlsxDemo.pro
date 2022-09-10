#---------------------------------------------------------
# 功能：       Qt使用QXlsx库读写Excel
# 编译器：
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/04/14
# @备注
#---------------------------------------------------------
QT       += core gui

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

#  定义程序版本号
VERSION = 1.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

include($$PWD/QXlsx/QXlsx.pri)             # QXlsx源代码，版本V1.4.3  https://github.com/QtExcel/QXlsx/releases
INCLUDEPATH += $$PWD/QXlsx
include($$PWD/TestXlsx/TestXlsx.pri)       # QXlsx使用Demo
INCLUDEPATH += $$PWD/TestXlsx

# 使用utf-8编码，这种用法好像只在msvc2017以后才有效
msvc {
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
}

RESOURCES += \
    appendix.qrc
