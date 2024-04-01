#---------------------------------------------------------------------------------------
# @功能：       QGraphicsItem基本图元演示
#              1、包含内置的 直线、矩形、椭圆、多边形、简单文本、富文本、图片、绘图路径、窗口部件基本图元；
#              2、使用自定义散点图元；
#              3、实现所有图元鼠标选中、移动功能，包括窗口部件图元；
#              4、解决自定义图元鼠标移动存在残留问题。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-09-02 23:17:22
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qgraphicspointsitem.cpp

HEADERS += \
    mainwindow.h \
    qgraphicspointsitem.h

FORMS += mainwindow.ui
RESOURCES += image.qrc

#  定义程序版本号
VERSION = 1.0.1
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

