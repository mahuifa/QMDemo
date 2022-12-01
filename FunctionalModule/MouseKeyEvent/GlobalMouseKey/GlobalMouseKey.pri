#---------------------------------------------------------------------------------------
# @功能：      封装的跨平台Qt全局鼠标、键盘事件监听器类
#             |     功能         |    系统    |  是否实现  |  实现方式       |
#             | 全局鼠标事件监听器  | Windows   |  已实现    | user32鼠标钩子  |
#             | 全局鼠标事件监听器  | Linux     |  已实现    | x11 API       |
#             | 全局键盘事件监听器  | Windows   |  已实现    | user32键盘钩子  |
#             | 全局键盘事件监听器  | Linux     |  未实现    |               |
#
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者      mhf
# @邮箱       1603291350@qq.com
# @时间       2022-12-01 15:31:51
# @备注       Linux下全局键盘事件监听器通过使用X11 API已经获取到了全局键盘事件，但是不知道怎么将
#            X11键盘事件映射为QKeyEvent，通过阅读Qt源码，发现Qt使用的是xcb将xkb映射为QKeyEvent，后续有时间在继续研究。
#            Qt源码位置：D:\Qt\Qt5.12.5\5.12.5\Src\qtbase\src\plugins\platforms\xcb\qxcbkeyboard.cpp
#---------------------------------------------------------------------------------------
QT       += concurrent

HEADERS += \
    $$PWD/globalkeyevent.h \           # 全局键盘事件监听器类头文件
    $$PWD/globalmouseevent.h           # 全局鼠标事件监听类头文件

SOURCES += \
    $$PWD/globalkeyevent_win.cpp \     # Windows全局键盘事件监听器
    $$PWD/globalkeyevent_x11.cpp \     # Linux全局键盘事件监听器（未实现）
    $$PWD/globalmouseevent_win.cpp \   # windows全局鼠标事件监听器
    $$PWD/globalmouseevent_x11.cpp     # linux全局鼠标事件监听器

win32 {
LIBS+= -luser32           # 使用WindowsAPI需要链接库
}
unix:!macx{
LIBS += -lX11 -lXtst      # linux获取鼠标、键盘事件信息需要用到xlib，Xtst 可以安装sudo apt install libxtst-dev
}
