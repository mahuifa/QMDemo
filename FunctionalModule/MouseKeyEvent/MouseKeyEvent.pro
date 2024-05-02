#---------------------------------------------------------------------------------------
# @功能：      全局鼠标、键盘事件示例
#             1、windows下使用鼠标钩子实现全局鼠标监听功能；
#             2、通过封装将Windows鼠标信号转换成Qt鼠标信号；
#             3、Linux下使用X11实现全局鼠标事件监听功能；
#             4、通过封装将X11鼠标信号转换为Qt鼠标信号（功能比Windows鼠标钩子强）；
#             5、Windows下实现全局键盘事件监听器功能；
#             6、Linux下通过x11 API获取到全局键盘事件，但是无法将x11键盘事件映射为QKeyEvent。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-11-13 22:23:08
# @备注
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

include($$PWD/GlobalMouseKey/GlobalMouseKey.pri)
INCLUDEPATH += $$PWD/GlobalMouseKey

#  定义程序版本号
VERSION = 1.8.0
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

