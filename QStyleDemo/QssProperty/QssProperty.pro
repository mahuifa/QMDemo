#---------------------------------------------------------------------------------------
# @功能：       QSS样式表使用---属性功能
# @编译器：     Desktop Qt 5.12.5 MinGW 64-bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2023-01-13 21:20:27
# @备注
#            1、使用**Qss属性选择器**设置通过【Q_PROPERTY】定义的属性的样式，当属性状态改变后控件样式跟着改变；
#            2、使用**Qss属性选择器**设置通过【setProperty()】定义的动态属性的样式，当属性状态改变后控件样式跟着改变；
#            3、使用 【qproperty-<属性名称>】语法设置通过Q_PROPERTY定义的属性的值；
#            4、定义一个枚举，使用Q_ENUM或者Q_ENUMS注册枚举类型，使用Q_PROPERTY定义一个已注册的枚举类型的属性，可以通过【qproperty-<属性名称>：枚举名称】方式设置属性的值；
#            5、在代码中不再是将样式表放在资源文件中，而是使用【INSTALLS】将样式表安装到可执行程序路径下，可定制性更强。
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += widget.h
FORMS += widget.ui

#  定义程序版本号
VERSION = 1.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
path = $$PWD/../bin             # 程序输出路径
}else{
path = $$PWD/../bin64           # 使用64位编译器
}
DESTDIR = $$path                # 指定编译后的文件存放路径

qssFile.path = $$path
qssFile.files = $$PWD/style.css
INSTALLS += qssFile      # 将qss文件拷贝到path路径下，需要配置【项目】->【Build】->【添加Build步骤】->【Custom Process Step:】 D:\Qt\Qt5.12.5\Tools\QtCreator\bin\jom.exe install才生效

# msvc >= 2017  编译器使用utf-8编码
msvc {
    greaterThan(QMAKE_MSC_VER, 1900){       # msvc编译器版本大于2015
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }else{
#        message(msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码)
    }
}
