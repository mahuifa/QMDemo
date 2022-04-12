QT += network
FORMS += \
    $$PWD/tcpclient.ui \
    $$PWD/tcpserver.ui

HEADERS += \
    $$PWD/share.h \
    $$PWD/tcpclient.h \                  # TCP 客户端Demo
    $$PWD/tcpserver.h                    # TCP 服务端Demo

SOURCES += \
    $$PWD/share.cpp \
    $$PWD/tcpclient.cpp \
    $$PWD/tcpserver.cpp
