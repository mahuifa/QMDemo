#---------------------------------------------------------------------------------------
# @功能：        Qt使用路径的方式和注意事项
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2023-01-06 14:00:29
# @备注       1、演示了Windows下、Linux下Qt使用各种相对路径、绝对路径、系统路径、快捷方式、软硬链接的方式和注意事项；
#            2、演示了Windows/Linux下直接运行程序和使用快捷方式、其它路径启动程序对路径的影响。
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += widget.h

FORMS += widget.ui

#  定义程序版本号
VERSION = 1.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
TARGET  = UsePath

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
