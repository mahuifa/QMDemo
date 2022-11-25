QT       += concurrent

HEADERS += \
    $$PWD/globalmouseevent.h           # 全局鼠标事件监听类头文件

SOURCES += \
    $$PWD/globalmouseevent_win.cpp \   # windows全局鼠标事件监听器
    $$PWD/globalmouseevent_x11.cpp     # linux全局鼠标事件监听器

win32 {
LIBS+= -luser32           # 使用WindowsAPI需要链接库
}
unix:!macx{
LIBS += -lX11 -lXtst      # linux获取鼠标、键盘事件信息需要用到xlib，Xtst 可以安装sudo apt install libxtst-dev
}
