#---------------------------------------------------------------------------------------
# @功能：       使用QWebEngineView加载显示Web页面的最小示例2
#             1、保留最简单功能，去掉不易于理解的功能；
#             2、分别演示了【QWebEngineView::setHtml】【QWebEngineView::setContent】的使用方法；
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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#  定义程序版本号
VERSION = 1.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

# msvc  编译器使用utf-8编码
msvc {
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
}
