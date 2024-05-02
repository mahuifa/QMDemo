#---------------------------------------------------------------------------------------
# @功能：       实现网页版WebSocket客户端程序
#             1、使用html编写界面、css设置样式、javascript实现webSocket客户端程序；
#             2、将html、css、js文件分开编写，便于阅读；
#             4、程序编译后自动将html文件夹拷贝到可执行程序当前路径；
#             5、可以直接点击chatClient.html文件运行客户端程序；
#             6、也可以在程序编译运行后使用QDesktopServices自动打开html文件。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-12-17 10:33:31
# @备注
#---------------------------------------------------------------------------------------
QT += core  widgets
CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNING
SOURCES += main.cpp

#  定义程序版本号
VERSION = 1.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
    DESTDIR = $$PWD/../bin             # 程序输出路径
}else{
    DESTDIR = $$PWD/../bin64           # 使用64位编译器
}

webFile.path = $$DESTDIR
webFile.files = $$PWD/html
INSTALLS += webFile      # 将html文件夹拷贝到path路径下，需要配置Custom Process Step: nmake install才生效
