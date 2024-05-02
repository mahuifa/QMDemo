#---------------------------------------------------------------------------------------
# @功能：      QPainter绘制时【数据类型】和【抗锯齿】对效果的影响
#             1、使用浮点数进行绘制，绘制的圆会更加均匀；
#             2、使用整形进行绘制，比浮点数计算快，适用于频繁、大量的重绘，但绘制效果较差；
#             3、开启抗锯齿后线条会更加平滑。
#             4、将布局改为使用设计器布局，减少使用代码布局，使关注点更多在绘制图形上；
#             5、将使用定时器更新绘制改为使用滑动条更新，增加可控性，便于观察绘制细节。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-07-25 05:44:58
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    circlewidget.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    circlewidget.h \
    widget.h

FORMS += widget.ui

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
