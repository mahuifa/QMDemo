#---------------------------------------------------------------------------------------
# @功能：       需要opencv是包含ffmpeg的，如果不包含则打开本地视频功能和网络视频功能无法使用
# @编译器：     MSVC2017-64（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-07-04 16:13:30
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui multimedia concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    playimage.cpp \
    videodisplay.cpp \
    widget.cpp

HEADERS += \
    playimage.h \
    videodisplay.h \
    widget.h

FORMS += \
    videodisplay.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# opencv需要带有ffmpeg才可以打开本地视频或者网络视频流，否则只能打开图片和摄像头
win32:CONFIG(release, debug|release): LIBS += -LE:/lib/opencv_MSVC2/build/x64/vc15/lib/ -lopencv_world460
else:win32:CONFIG(debug, debug|release): LIBS += -LE:/lib/opencv_MSVC2/build/x64/vc15/lib/ -lopencv_world460d
INCLUDEPATH += E:/lib/opencv_MSVC2/build/include
DEPENDPATH += E:/lib/opencv_MSVC2/build/include

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}
# msvc  编译器使用utf-8编码（这种方式好像在msvc2017以后版本才有效）
msvc {
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
}

