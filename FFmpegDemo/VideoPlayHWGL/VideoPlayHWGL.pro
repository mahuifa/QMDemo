#---------------------------------------------------------------------------------------
# @功能：       使用ffmpeg音视频库【软/硬解码 + OpenGL】实现的视频播放器；
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit 32bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-09-14 14:13:52
# @备注       1、使用ffmpeg音视频库【软/硬解码】实现的视频播放器；
#             2、支持打开本地视频文件（如mp4、mov、avi等）、网络视频流（rtsp、rtmp、http等）；
#             3、支持视频匀速播放；
#             4、采用【OpenGL显示YUV、NV12】图像，支持自适应窗口缩放，支持使用QOpenGLWidget、QOpenGLWindow显示；
#             5、将YUV/NV12转RGB的步骤由CPU转换改为使用GPU转换，降低CPU占用率；
#             6、使用av_hwframe_map替代av_hwframe_transfer_data，可将【耗时降低1/3】；
#             7、视频播放支持实时开始/关闭、暂停/继续播放；
#             8、视频解码、线程控制、显示各部分功能分离，低耦合度。
#             9、采用最新的【5.1.2版本】ffmpeg库进行开发，超详细注释信息，将所有踩过的坑、解决办法、注意事项都得很写清楚。
#---------------------------------------------------------------------------------------
QT       += core gui

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

FORMS += widget.ui

# ffmpeg读取视频图像模块
include(./VideoPlay/VideoPlay.pri)
INCLUDEPATH += ./VideoPlay

#  定义程序版本号
VERSION = 1.0.3
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
TARGET  = VideoPlayHWGL

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
    # msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码
    }
}

DISTFILES += \
    fragment.fsh \
    vertex.vsh

RESOURCES += \
    rc.qrc
