#---------------------------------------------------------------------------------------
# @功能：       NTP时间同步客户端程序Demo
#              1、使用UDP进行通信；
#              2、毫秒级时间精度；
#              3、使用多个阿里云NTP时间同步服务器、腾讯云NTP时间同步服务器；
#              4、支持windows、linux下修改系统时间。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-08-09 15:52:56
# @备注       注意：由于设置系统时间的功能比较重要，所以不管是Windows还是Linux都需要最高权限才可以。
#             Windows下需要【以管理员身份运行】打开QtCreator或者编译后给NtpClient.exe设置权限【属性->兼容性->以管理员身份运行此程序】，否则无法修改系统时间；
#             Linux下编译后使用【sudo ./NtpClient】 运行程序。
#---------------------------------------------------------------------------------------
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    main.cpp \
    ntpclient.cpp \
    widget.cpp

HEADERS += \
    ntpclient.h \
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

# msvc >= 2017  编译器使用utf-8编码
msvc {
#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"          # 设置程序使用管理员权限运行（只适用于MSVC）
    greaterThan(QMAKE_MSC_VER, 1900){       # msvc编译器版本大于2015
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }else{
#        message(msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码)
    }
}
