#---------------------------------------------------------------------------------------
# @功能：       需要opencv是包含ffmpeg的，如果不包含则打开本地视频功能和网络视频功能无法使用
#              1、支持打开本地摄像头；
#              2、支持打开网络视频流，如rtsp、rtmp、http等；
#              3、支持打开本地视频文件；
#              4、支持显示图像自适应大小；
#              5、支持将读取到的视频保存到本地；
# @编译器：     MSVC2017-64（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-07-04 16:13:30
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui multimedia concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    playimage.cpp \
    videodisplay.cpp \
    widget.cpp

HEADERS += \
    playimage.h \
    videodisplay.h \
    widget.h

FORMS += \
    videodisplay.ui \
    widget.ui

#  定义程序版本号
VERSION = 1.1.3
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

# 连接opencv库，opencv需要带有ffmpeg才可以打开本地视频或者网络视频流，否则只能打开图片和摄像头
win32{
    opencvPath = E:/lib/opencv_MSVC/   # 设置Opencv库路径
    CONFIG(release, debug|release){   # Release模式
        LIBS += -L$$opencvPath/build/x64/vc15/lib/ -lopencv_world3416  # 设置引入opencv库
        opencvDLL.files += $$opencvPath/build/x64/vc15/bin/opencv_ffmpeg3416_64.dll
        opencvDLL.files += $$opencvPath/build/x64/vc15/bin/opencv_world3416.dll
    }else:CONFIG(debug, debug|release){ # Debug模式
        LIBS += -L$$opencvPath/build/x64/vc15/lib/ -lopencv_world3416d
        opencvDLL.files += $$opencvPath/build/x64/vc15/bin/opencv_ffmpeg3416_64.dll
        opencvDLL.files += $$opencvPath/build/x64/vc15/bin/opencv_world3416d.dll
#        opencvDLL.files += $$opencvPath/build/x64/vc15/bin/opencv_world460.pdb
#        opencvDLL.files += $$opencvPath/build/x64/vc15/bin/opencv_world460d.pdb
    }
    INCLUDEPATH += $$opencvPath/build/include
    DEPENDPATH += $$opencvPath/build/include
    # 自动安装依赖文件和库文件
    opencvDLL.path = $$DESTDIR

    # 方法1：msvc需要配置【Custom Process Step: nmake install】或者【Custom Process Step: D:\Qt\Qt5.12.5\Tools\QtCreator\bin\jom.exe install】才生效，或者自己手动拷贝
    # 方法2：msvc需要配置【项目】【运行】【添加部署步骤】【make】,参数输入【install】
    # Debug和Release需要分别配置
    # 执行之前先qmake，如果不想每次手动qmake，可以点击【工具】->【选项】->【构建和运行】->【qmake】->勾选【Run qmake every build】
    INSTALLS += opencvDLL      # 将opencv库文件拷贝到path路径下
}
unix:!macx{
}


# msvc >= 2017  编译器使用utf-8编码
msvc {
    greaterThan(QMAKE_MSC_VER, 1900){       # msvc编译器版本大于2015
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }else{
#        message(msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码)
    }
}

