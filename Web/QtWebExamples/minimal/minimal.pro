#---------------------------------------------------------------------------------------
# @功能：       使用QWebEngineView加载显示Web页面的最小示例
#             1、保留最简单功能，去掉不易于理解的功能；
#             2、分别演示了加载网络地址、资源文件中的Html文件，本地html的方式；
#             3、详细的注释，对初学者更友好。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-12-10 20:22:53
# @备注
#---------------------------------------------------------------------------------------
TEMPLATE += app
QT += webenginewidgets

DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += main.cpp
RESOURCES += rc.qrc

#  定义程序版本号
VERSION = 1.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
    DESTDIR = $$PWD/../bin             # 程序输出路径
}else{
    DESTDIR = $$PWD/../bin64           # 使用64位编译器
}

webFile.path = $$DESTDIR
webFile.files = $$PWD/hello.html
INSTALLS += webFile      # 将hello.html文件拷贝到path路径下，需要配置Custom Process Step: nmake install才生效

# msvc >= 2017  编译器使用utf-8编码
msvc {
    greaterThan(QMAKE_MSC_VER, 1900){       # msvc编译器版本大于2015
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }else{
#        message(msvc2015及以下版本在代码中使用【pragma execution_character_set("utf-8")】指定编码)
    }
}
