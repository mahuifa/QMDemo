#---------------------------------------------------------
# 功能：      Qt崩溃定位Demo
# 编译器：    MSVC
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/04/05
# @备注
#---------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
message("64bit")                # 使用64位编译器
DESTDIR = $$PWD/../bin64
}

include($$PWD/CrashHandler/CrashHandler.pri)
INCLUDEPATH += $$PWD/CrashHandler
