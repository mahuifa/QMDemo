#---------------------------------------------------------------------
# 模块功能：  qt日志系统模块，无第三方依赖
#            1、支持将日志保存到纯文本Log中；
#            2、支持将日志保存到纯文本的CSV中，便于阅读和查找日志信息
# 支持编译器：
# 开发者：    mhf
# 邮箱      1603291350@qq.com
# 时间：     2022/03/27
#---------------------------------------------------------------------
DEFINES += QT_MESSAGELOGCONTEXT        # release模式下输出日志
DEFINES += OUT_TERMINAL                # 日志输出到终端terminal

FORMS += \
    $$PWD/logwidgettext.ui

HEADERS += \
    $$PWD/logconfig.h \
    $$PWD/loginput.h \
    $$PWD/logsavebase.h \
    $$PWD/logsavetxt.h \
    $$PWD/logwidgetbase.h \
    $$PWD/logwidgettext.h

SOURCES += \
    $$PWD/logconfig.cpp \
    $$PWD/loginput.cpp \
    $$PWD/logsavebase.cpp \
    $$PWD/logsavetxt.cpp \
    $$PWD/logwidgetbase.cpp \
    $$PWD/logwidgettext.cpp

