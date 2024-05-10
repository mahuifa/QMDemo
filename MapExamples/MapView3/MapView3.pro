#---------------------------------------------------------------------------------------
# @功能：       Qt使用QGraphicsView显示在线瓦片地图简单示例（绝对像素坐标）
# @编译器：     msvc2017_64（也支持其它编译器）
# @Qt IDE：    D:/Qt/qtcreator-10.0.2/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2024-05-07 22:34:22
# @备注       1、加载显示在线瓦片地图；
#             2、支持以鼠标为原点缩放瓦片层级；
#             3、使用绝对像素坐标方式显示瓦片；
#             4、支持在线程池中快速下载在线瓦片；
#             5、支持鼠标缩放地图层级；
#             6、支持显示瓦片编号、瓦片网格；
#             7、默认支持下载显示多格式高德、Bing、ArcGis瓦片地图。
#---------------------------------------------------------------------------------------


QT       += core gui  network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

include($$PWD/MapView/MapView.pri)
INCLUDEPATH += $$PWD/MapView/

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

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
