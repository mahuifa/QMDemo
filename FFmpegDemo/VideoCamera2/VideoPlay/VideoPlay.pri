#---------------------------------------------------------------------------------------
# @功能：       ffmpeg读取视频图像模块
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-09-15 14:27:25
# @备注
#---------------------------------------------------------------------------------------

# 加载库，ffmpeg n5.1.2版本
win32{
LIBS += -LE:/lib/ffmpeg5-1-2/lib/ -lavcodec -lavfilter -lavformat -lswscale -lavutil -lswresample -lavdevice
INCLUDEPATH += E:/lib/ffmpeg5-1-2/include
DEPENDPATH += E:/lib/ffmpeg5-1-2/include

#INCLUDEPATH += E:/lib/ffmpeg4/include
#LIBS += -LE:/lib/ffmpeg4/lib/win64/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale
}

unix:!macx{
LIBS += -L/home/mhf/lib/ffmpeg/ffmpeg-5-1-2/lib -lavcodec -lavfilter -lavformat -lswscale -lavutil -lswresample -lavdevice
INCLUDEPATH += /home/mhf/lib/ffmpeg/ffmpeg-5-1-2/include
DEPENDPATH += /home/mhf/lib/ffmpeg/ffmpeg-5-1-2/include
}

HEADERS += \
    $$PWD/readthread.h \        # 视频读取线程类
    $$PWD/videodecode.h \       # ffmpeg解码类
    $$PWD/videosave.h           # ffmpeg编码保存视频类

SOURCES += \
    $$PWD/readthread.cpp \
    $$PWD/videodecode.cpp \
    $$PWD/videosave.cpp
