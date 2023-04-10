#---------------------------------------------------------------------------------------
# @功能：      测试比较QOpenGLWidget和QOpenGLWindow性能
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-09-28 17:20:12
# @备注       1、使用OpenGl 1.0 API绘制三角形（实现简单，已经淘汰的API）;
#            2、使用自定义Shader脚本绘制三角形（实现较为复杂）
#            3、分别使用QOpenGLWindow和QOpenGLWidget绘制三角形；
#            4、分别使用定时器、子线程刷新绘制；
#            5、通过比较，QOpenGLWindow性能强于QOpenGLWidget。
#---------------------------------------------------------------------------------------
QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    main.cpp \
    openglwidget.cpp \
    openglwindow.cpp \
    widget.cpp

HEADERS += \
    openglwidget.h \
    openglwindow.h \
    widget.h

FORMS += widget.ui
DISTFILES += fragmentShader.fsh
DISTFILES += vertexShader.vsh
RESOURCES += Shader.qrc

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
        message(msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码)
    }
}
