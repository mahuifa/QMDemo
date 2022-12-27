
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
    $$PWD/readthread.h \
    $$PWD/videocodec.h \
    $$PWD/videodecode.h

SOURCES += \
    $$PWD/readthread.cpp \
    $$PWD/videocodec.cpp \
    $$PWD/videodecode.cpp
