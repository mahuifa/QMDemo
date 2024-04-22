#---------------------------------------------------------------------------------------
# @功能：       使用QtWebChannel实现Qt与Web双向交互
#             1、构建后将html、css、js文件自动拷贝到可执行程序路径下；
#             2、web界面和qt界面实现双向通信。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-12-10 21:57:06
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui webenginewidgets webchannel

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += widget.h
FORMS += widget.ui

#  定义程序版本号
VERSION = 1.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
    DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
    DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

# 程序编译后需要使用nmake install（msvc）或make install (linux)将web文件夹拷贝到当前路径下，或者自己手动拷贝
webFile.path = $$DESTDIR
webFile.files = $$PWD/web
INSTALLS += webFile      # 将web文件夹拷贝到path路径下，需要配置Custom Process Step: nmake install才生效

# msvc >= 2017  编译器使用utf-8编码
msvc {
    greaterThan(QMAKE_MSC_VER, 1900){       # msvc编译器版本大于2015
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }else{
#        message(msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码)
    }
}
