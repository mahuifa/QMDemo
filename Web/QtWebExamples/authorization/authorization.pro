#---------------------------------------------------------------------------------------
# @功能：       QWebEnginePage权限管理
#              1、使用QWebEnginePage有时需要权限，例如打开摄像头、麦克风；
#              2、这里演示了怎么实现授权功能；
#              3、分别展示了打开网络地址授权和打开本地html授权。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-12-08 16:05:25
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += mainwindow.h
RESOURCES += rc.qrc


#  定义程序版本号
VERSION = 1.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
    DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
    DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

webFile.path = $$DESTDIR
webFile.files = $$PWD/video.html
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
