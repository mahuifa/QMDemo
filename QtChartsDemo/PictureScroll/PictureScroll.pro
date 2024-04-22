#---------------------------------------------------------------------------------------
# @功能：       画卷式散点图，每个散点绑定一张图片
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2023-05-03 21:36:55
# @备注      1、X轴固定，Y轴随坐标点自动修改显示范围，滚动显示；
#            2、将图片与坐标点绑定，点击坐标点显示对应图片；
#            3、通过滑动条滑动显示区域；
#            4、通过鼠标滚轮缩放Y轴显示范围；
#            5、可将坐标点、图片格式化保存到csv文件；
#            6、支持清空显示；
#            7、支持从CSV文件加载数据显示。
#---------------------------------------------------------------------------------------
QT       += core gui charts concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    chartview.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    chartview.h \
    widget.h

FORMS += widget.ui

RESOURCES += img.qrc

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

