#---------------------------------------------------------------------------------------
# @功能：      FFmpeg录屏模块
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-12-28 22:17:35
# @备注
#---------------------------------------------------------------------------------------
# 加载库，ffmpeg n5.1.2版本
win32{
LIBS += -LE:/lib/ffmpeg5-1-2/lib/ -lavcodec -lavfilter -lavformat -lswscale -lavutil -lswresample -lavdevice
INCLUDEPATH += E:/lib/ffmpeg5-1-2/include
DEPENDPATH += E:/lib/ffmpeg5-1-2/include
}

unix:!macx{
LIBS += -L/home/mhf/lib/ffmpeg/ffmpeg-5-1-2/lib -lavcodec -lavfilter -lavformat -lswscale -lavutil -lswresample -lavdevice
INCLUDEPATH += /home/mhf/lib/ffmpeg/ffmpeg-5-1-2/include
DEPENDPATH += /home/mhf/lib/ffmpeg/ffmpeg-5-1-2/include
}

HEADERS += \
    $$PWD/readthread.h \       # 录屏线程类
    $$PWD/videocodec.h \       # 录屏编码类（将图像保存到视频文件中）
    $$PWD/videodecode.h        # 录屏解码类（捕获桌面图像并解码）

SOURCES += \
    $$PWD/readthread.cpp \
    $$PWD/videocodec.cpp \
    $$PWD/videodecode.cpp
