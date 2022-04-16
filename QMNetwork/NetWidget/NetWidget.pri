QT += network
FORMS += \
    $$PWD/tcpclient.ui \
    $$PWD/tcpserver.ui \
    $$PWD/udpsocket.ui

HEADERS += \
    $$PWD/share.h \
    $$PWD/tcpclient.h \                    # TCP 客户端Demo
    $$PWD/tcpserver.h \                    # TCP 服务端Demo
    $$PWD/udpsocket.h

SOURCES += \
    $$PWD/share.cpp \
    $$PWD/tcpclient.cpp \
    $$PWD/tcpserver.cpp \
    $$PWD/udpsocket.cpp
