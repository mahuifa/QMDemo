#---------------------------------------------------------------------------------------
# @功能：      libavformat AVIOContext API示例。
#             使libavformat解复用器通过自定义AVIOContext读取回调访问媒体内容。
#             1. 将一个视频文件中说有数据读取到buf中；
#             2. 为AVIOContext创建一个回调函数；
#             3. 创建一个长度为4096内存用于从buf中读取数据；
#             4. 使用回调函数完成数据的读取。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-10-23 19:27:48
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += main.cpp
SOURCES += widget.cpp

HEADERS += widget.h
FORMS += widget.ui

#  定义程序版本号
VERSION = 1.0.3
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
TARGET  = AVIOReading

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

# 加载库，ffmpeg n5.1.2版本
win32{
LIBS += -LE:/lib/ffmpeg5-1-2/lib/ -lavcodec -lavfilter -lavformat -lswscale -lavutil -lswresample -lavdevice
INCLUDEPATH += E:/lib/ffmpeg5-1-2/include
DEPENDPATH += E:/lib/ffmpeg5-1-2/include
}

unix:!macx{
LIBS += -L/home/mhf/lib/ffmpeg/ffmpeg-5-1-2/lib -lavcodec -lavfilter -lavformat -lswscale -lavutil -lswresample -lavdevice
INCLUDEPATH += /home/mhf/lib/ffmpeg/ffmpeg-5-1-2/include
DEPENDPATH += /home/mhf/lib/ffmpeg/ffmpeg-5-1-2/include
}

# 自动安装依赖文件和库文件（安装一次就够了，如果每个pro文件都写那就会安装多次，非常浪费时间）
ffmpegDLL.path = $$DESTDIR
ffmpegDLL.files = E:/lib/ffmpeg5-1-2/bin/*.dll

# msvc需要配置【Custom Process Step: nmake install】或者【Custom Process Step: D:\Qt\Qt5.12.5\Tools\QtCreator\bin\jom.exe install】才生效，或者自己手动拷贝
# Debug和Release需要分别配置
# 执行之前先qmake，如果不想每次手动qmake，可以点击【工具】->【选项】->【构建和运行】->【qmake】->勾选【Run qmake every build】
INSTALLS += ffmpegDLL      # 将hello.html拷贝到path路径下
