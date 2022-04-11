QT += network
FORMS += \
    $$PWD/tcpclient.ui \
    $$PWD/tcpserver.ui

HEADERS += \
    $$PWD/share.h \
    $$PWD/tcpclient.h \ \                 # TCP Client
    $$PWD/tcpserver.h

SOURCES += \
    $$PWD/share.cpp \
    $$PWD/tcpclient.cpp \
    $$PWD/tcpserver.cpp
