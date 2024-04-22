#---------------------------------------------------------------------------------------
# @功能：      使用QWebSocket客户端作为QWebChannel通信数据传输接口实现Qt和Web端程序交互功能
#             QWebChannel默认使用的传输通道为WebChannelIPCTransportHost，内部的数据传输接口为qtwebchannel::mojom::WebChannelTransportRenderAssociatedPtr
#             WebChannelIPCTransportHost所在文件：D:\Qt\Qt5.12.5\5.12.5\Src\qtwebengine\src\core\renderer_host\web_channel_ipc_transport_host.h
#             1、构建后将html、css、js文件自动拷贝到可执行程序A路径下；
#             2、使用QWebSocket作为QWebChannel通信接口；
#             3、通过QWebEngineView在Qt界面中嵌入Web网页；
#             4、一键调用浏览器打开html文件；
#             5、基于WebSocket、QWebChannel.js实现网页和Qt通信功能；
#             6、同时演示了Qt和嵌入网页界面、浏览器网页界面通信功能;
#             7、支持一个服务端与多个客户端Web界面进行通信交互，将WebSocket服务端封装在QWebChannel内部，一个WebChannel对应一个服务端、多个客户端。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-12-17 21:31:51
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui webchannel websockets webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    main.cpp \
    swebchannel.cpp \
    websockettransport.cpp \
    widget.cpp

HEADERS += \
    swebchannel.h \
    websockettransport.h \
    widget.h

FORMS += widget.ui

#  定义程序版本号
VERSION = 1.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
    DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
    DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

# 程序编译后需要使用nmake install（msvc）或make install (linux)将web2文件夹拷贝到当前路径下，或者自己手动拷贝
webFile.path = $$DESTDIR
webFile.files = $$PWD/web3
INSTALLS += webFile      # 将web文件夹拷贝到path路径下，需要配置Custom Process Step: nmake install才生效

# msvc >= 2017  编译器使用utf-8编码
msvc {
    greaterThan(QMAKE_MSC_VER, 1900){       # msvc编译器版本大于2015
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }else{
#        message(msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码)
    }
}
