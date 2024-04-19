#---------------------------------------------------------
# 功能：       使用QPainter方式绘制图片，显示效率强于使用QLabel显示，并且不像opengl、
#             D3D一样对环境有依赖，换一个设备就可能出问题了（当然技术好可以使得代码适应性强），
#             在这个Demo中同时显示了8 * 8共64路图片，显示频率50Hz
# 优点：       使用简单、适用于所有平台，所有编译器，没有第三方依赖
# 编译器：     Desktop Qt 5.12.12 MSVC2017 64bit（也支持其它编译器）
# Qt IDE：    D:/Qt/Qt5.12.12/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-05-05 22:02:36
# @备注
#---------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    playimage.cpp \
    widget.cpp

HEADERS += \
    playimage.h \
    widget.h

FORMS += widget.ui

RESOURCES += image.qrc


#  定义程序版本号
VERSION = 1.0.3
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
