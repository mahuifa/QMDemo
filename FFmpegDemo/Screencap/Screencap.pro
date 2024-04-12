#---------------------------------------------------------------------------------------
# @功能：       使用FFmpeg实现录屏功能
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-12-25 20:53:58
# @备注       1、抓取桌面图像转码后保存到本地视频文件中；
#            2、支持各种常见视频文件类型；
#            3、支持Windows、Linux录屏功能；
#            4、主要功能分为录屏线程、录屏解码、图像像素转换、编码保存4部分。
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp
SOURCES += widget.cpp

HEADERS += widget.h
FORMS += widget.ui

include(./Screencap/Screencap.pri)
INCLUDEPATH += ./Screencap

#  定义程序版本号
VERSION = 1.0.4
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
    # msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码
    }
}
