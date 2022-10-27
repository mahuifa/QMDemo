#---------------------------------------------------------------------------------------
# @功能：       使用ffmpeg音视频库【软解码】打开本地摄像头；
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit 32bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-09-14 14:13:52
# @备注       1、使用ffmpeg音视频库【软解码】打开本地摄像头；
#             2、采用【OpenGL显示YUV】图像，支持自适应窗口缩放，支持使用QOpenGLWidget、QOpenGLWindow显示；
#             3、将YUV转RGB的步骤由CPU转换改为使用GPU转换，降低CPU占用率；
#             4、视频播放支持实时开始/关闭、暂停/继续播放；
#             5、视频解码、线程控制、显示各部分功能分离，低耦合度。
#             6、采用最新的5.1.2版本ffmpeg库进行开发，超详细注释信息，将所有踩过的坑、解决办法、注意事项都得很写清楚。
#---------------------------------------------------------------------------------------
QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    playimage.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    playimage.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# ffmpeg读取视频图像模块
include(./VideoPlay/VideoPlay.pri)
INCLUDEPATH += ./VideoPlay

#  定义程序版本号
VERSION = 1.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
TARGET  = VideoCamera1

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
    fragment.fsh \
    vertex.vsh

RESOURCES += \
    rc.qrc
