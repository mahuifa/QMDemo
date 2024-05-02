#---------------------------------------------------------------------------------------
# @功能：       框选鼠标当前位置窗口范围（类似窗口截图）
#              1.使用WindowsAPI实现windows下功能；
#              2.使用x11 API实现linux（ubuntu）下功能；
#              3.windows下使用鼠标钩子解决鼠标穿透后无法获取鼠标点击事件问题。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-11-10 20:32:00
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += main.cpp
SOURCES += widget.cpp

HEADERS += widget.h

#  定义程序版本号
VERSION = 1.2.1
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

win32 {
LIBS+= -luser32    # 使用WindowsAPI需要链接库
}
unix:!macx{
LIBS += -lX11      # linux获取窗口信息需要用到xlib
}

