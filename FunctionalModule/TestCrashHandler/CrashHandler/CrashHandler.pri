#---------------------------------------------------------
# 功能：      使用win api捕获qt崩溃异常信息，定位崩溃位置
# 编译器：    MSVC
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/04/05
# @备注       无需引入 第三方依赖
#---------------------------------------------------------
HEADERS += \
    $$PWD/crashhandler.h

SOURCES += \
    $$PWD/crashhandler.cpp

LIBS += -ldbghelp
# msvc(VS调试模式，在VS2015 + QT5.6.3环境下调试通过)
QMAKE_CXXFLAGS_RELEASE += /Zi
QMAKE_CXXFLAGS_RELEASE += /Od
QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG  # Release版也将生成“.pdb”后缀的调试信息文件

## DEFINES +=QT_NO_DEBUG_OUTPUT # disable debug output
DEFINES -= QT_NO_DEBUG_OUTPUT # enable debug output
