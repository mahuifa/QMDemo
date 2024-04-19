#---------------------------------------------------------------------------------------
# @功能：       QPainter基本绘图Demo
#              1. 绘制原点和坐标轴；
#              2. 绘制线段、散点、多线段（折线）、多边形、矩形、圆角矩形、椭圆、弧线、弦形、扇形、绘制路径、文字、图片；
#              3. 可设置画笔样式、笔帽样式、线条连接样式、笔刷样式、画笔粗细、颜色、抗锯齿；
#              4. 可设置坐标原点水平、垂直偏移、旋转、缩放。
# @编译器：     MSVC2017-64（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-07-18 20:23:48
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    renderarea.cpp \
    widget.cpp

HEADERS += \
    renderarea.h \
    widget.h

FORMS += widget.ui
RESOURCES += image.qrc

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

