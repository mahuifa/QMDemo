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
    main.cpp \
    openglwidget.cpp \
    openglwindow.cpp \
    widget.cpp

HEADERS += \
    openglwidget.h \
    openglwindow.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#  定义程序版本号
VERSION = 1.0.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}
# msvc  编译器使用utf-8编码
msvc {
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
}

DISTFILES += \
    fragmentShader.fsh \
    vertexShader.vsh

RESOURCES += \
    Shader.qrc
