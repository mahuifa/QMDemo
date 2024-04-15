#---------------------------------------------------------------------------------------
# @功能：      在QT子线程中操作IO对象，包括QAbstractSocket、QFile、QSerialPort等
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2023-05-17 11:55:27
# @备注      在Qt框架中，QIODevice及其子类（如QSerialPort、QTcpSocket等）设计用于单线程内的操作。
#            如果尝试在创建QIODevice对象的线程之外的另一个线程中使用它，将会引发错误或不可预期的行为。
#            这是因为QIODevice依赖于Qt的信号和槽机制进行事件处理和数据通信，而这些机制本身不是线程安全的。
#            但有时候实际场景中数据量很大，就需要在子线程中进行通信，这里就演示如何在子线程中使用QIODevice
#---------------------------------------------------------------------------------------
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
SOURCES += \
    iothread.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    iothread.h \
    widget.h

FORMS += widget.ui

#  定义程序版本号
VERSION = 1.0.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

# msvc >= 2017  编译器使用utf-8编码
msvc {
    greaterThan(QMAKE_MSC_VER, 1900){       # msvc编译器版本大于2015
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }else{
        # msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码
    }
}
