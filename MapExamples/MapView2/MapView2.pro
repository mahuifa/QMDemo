#---------------------------------------------------------------------------------------
# @功能：      Qt使用QGraphicsView显示瓦片地图简单示例（绝对像素坐标）
# @编译器：     Desktop Qt 5.14.2 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.14.2/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2024-04-13 10:33:17
# @备注       1、加载显示本地离线瓦片；
#             2、使用绝对像素坐标方式显示瓦片；
#             3、支持以鼠标为原点缩放瓦片层级；
#             4、支持在线程池中快速加载本地瓦片；
#             5、为了示例更加简单，复杂功能没在这个示例实现，会加载当前层级所有瓦片。
#---------------------------------------------------------------------------------------

QT       += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

include($$PWD/MapView/MapView.pri)
INCLUDEPATH += $$PWD/MapView/

#  定义程序版本号
VERSION = 1.0.0
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
