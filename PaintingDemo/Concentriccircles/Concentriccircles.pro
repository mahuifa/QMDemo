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

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    circlewidget.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    circlewidget.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

# msvc  编译器使用utf-8编码（好像只在msvc2017以后才有效）
msvc {
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
}
