#---------------------------------------------------------------------------------------
# @功能：       Qt使用cef加载网页简单示例
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2023-02-05 12:08:27
# @备注       在父工程中定义依赖库的路径CefPath，如果直接编译当前工程，则需要指定CefPath
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += widget.h
FORMS += widget.ui

#  定义程序版本号
VERSION = 1.0.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# 程序输出路径
contains(QT_ARCH, i386){        # 使用32位编译器
    CONFIG(release, debug|release){
        DESTDIR = $$PWD/../bin
    }else:CONFIG(debug, debug|release){
        DESTDIR = $$PWD/../bind
    }
}else{
    CONFIG(release, debug|release){
        DESTDIR = $$PWD/../bin64
    }else:CONFIG(debug, debug|release){
        DESTDIR = $$PWD/../bin64d
    }
}

# CEF 库的路径， 如果路径为空则不编译当前工程
win32{
    CefPath = E:/lib/QCefView
}
unix:!macx{
}

# 导入库头文件的路径
INCLUDEPATH += $$CefPath/include
DEPENDPATH += $$CefPath/include

# Debug和release的库路径不同
CONFIG(release, debug|release){
    cefDLL.files = $$CefPath/bin/*
    LIBS += -L$$CefPath/lib/ -lQCefView
}else:CONFIG(debug, debug|release){
    cefDLL.files = $$CefPath/bind/*
    LIBS += -L$$CefPath/libd/ -lQCefView
}

# 自动安装依赖文件和库文件
cefDLL.path = $$DESTDIR
webFile.path = $$DESTDIR
webFile.files = $$PWD/hello.html

# msvc需要配置【Custom Process Step: nmake install】或者【Custom Process Step: D:\Qt\Qt5.12.5\Tools\QtCreator\bin\jom.exe install】才生效，或者自己手动拷贝
# Debug和Release需要分别配置
# 执行之前先qmake，如果不想每次手动qmake，可以点击【工具】->【选项】->【构建和运行】->【qmake】->勾选【Run qmake every build】
!exists($$webFile.path/hello.html): INSTALLS += webFile      # 将hello.html拷贝到path路径下
!exists($$cefDLL.path/QCefView.dll): INSTALLS += cefDLL      # 将CEF库文件拷贝到path路径下

# msvc >= 2017  编译器使用utf-8编码
msvc {
    greaterThan(QMAKE_MSC_VER, 1900){       # msvc编译器版本大于2015
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }else{
#        message(msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码)
    }
}
