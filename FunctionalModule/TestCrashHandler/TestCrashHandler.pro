#---------------------------------------------------------
# 功能：      Qt崩溃定位Demo
# 编译器：    MSVC（MinGW、GCC不生效）
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/04/05
# @备注
#---------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

#  定义程序版本号
VERSION = 1.1.0
TARGET = TestCrash_$$VERSION
DEFINES += APP_VERSION=\\\"$$VERSION\\\"   # 程序版本号
DEFINES += APP_NAME=\\\"$$TARGET\\\"      # 程序名称字符串

include($$PWD/CrashHandler/CrashHandler.pri)
INCLUDEPATH += $$PWD/CrashHandler

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += widget.h

FORMS += widget.ui

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

