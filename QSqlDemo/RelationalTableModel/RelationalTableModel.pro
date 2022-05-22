#---------------------------------------------------------------------------------------
# 功能：       本示例展示了如何使用QSqlRelationalTableModel（关系表模型）来可视化数据库中[外键]的使用。
# 编译器：     Desktop Qt 5.12.12 MSVC2017 64bit（也支持其它编译器）
# Qt IDE：    D:/Qt/Qt5.12.12/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-05-14 14:02:25
# @备注
#---------------------------------------------------------------------------------------
QT       += core sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
SOURCES += main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
message("64bit")                # 使用64位编译器
DESTDIR = $$PWD/../bin64
}

# msvc  编译器使用utf-8编码
msvc {
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
}
