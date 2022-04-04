#---------------------------------------------------------
# 功能：      自定义播放器界面
             1、自定义窗口；
             2、侧边栏、控制栏、进度条显示隐藏动画；
# 编译器：
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/04/04
# @备注
#---------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11


SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

RESOURCES += \
    res.qrc


# 加载模块
include($$PWD/WidgetBase/WidgetBase.pri)       # 自定义窗口模块
include($$PWD/Module/Module.pri)               # 界面组件

INCLUDEPATH += $$PWD/WidgetBase/
INCLUDEPATH += $$PWD/Module/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#  定义程序版本号
VERSION = 0.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
message("64bit")                # 使用64位编译器
DESTDIR = $$PWD/../bin64
}
