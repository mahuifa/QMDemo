#---------------------------------------------------------
# 功能：      使用windows下的dbghelp api捕获qt崩溃异常信息，定位崩溃位置
# 编译器：    MSVC
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/04/05
# @备注       无需引入 第三方依赖
#            虽然MSVC、MinGW都能运行并生成dump文件，但是由于Mingw编译的程序不会生成
#            pdb符号文件，不方便调试，所以建议使用MSVC编译器
#---------------------------------------------------------
HEADERS += $$PWD/crashhandler.h

SOURCES += $$PWD/crashhandler.cpp

LIBS += -ldbghelp

msvc:CONFIG(release, debug|release) {
    QMAKE_CFLAGS_RELEASE   -= -O2              # 取消C优化
    QMAKE_CFLAGS_RELEASE   += -Zi              # 生成调试信息，放到pdb文件中
    QMAKE_CXXFLAGS_RELEASE -= -O2              # 取消C++优化
    QMAKE_CXXFLAGS_RELEASE += -Zi              # 生成调试信息
    QMAKE_LFLAGS_RELEASE   -= /INCREMENTAL:NO  # 选择增量链接
    QMAKE_LFLAGS_RELEASE   += /DEBUG           # 将调试信息放到PDB文件中
#    message("MSVC编译器Release关闭优化，生成调试信息使用"")
}

mingw:CONFIG(release, debug|release) {
    QMAKE_CFLAGS_RELEASE   -= -O2              # 取消C优化
    QMAKE_CFLAGS_RELEASE   += -O0              # 显示指定禁止优化
    QMAKE_CFLAGS_RELEASE   += -g               # 生成C调试信息
    QMAKE_CXXFLAGS_RELEASE -= -O2              # 取消C++优化
    QMAKE_CXXFLAGS_RELEASE += -O0              # 显示指定禁止优化
    QMAKE_CXXFLAGS_RELEASE += -g               # 生成C++调试信息
    QMAKE_LFLAGS_RELEASE   -= -Wl,-s           # 取消Release模式删除所有符号表和重新定位信息的设置
    QMAKE_LFLAGS_RELEASE   += -g               # 链接器生成调试信息
#    message("Mingw编译器Release关闭优化，生成调试信息使用"")
}

# 如果不加unix，MinGW也会进入这里
unix:gcc:CONFIG(release, debug|release) {
    QMAKE_CFLAGS_RELEASE   -= -O2              # 取消C优化
    QMAKE_CFLAGS_RELEASE   += -O0              # 显示指定禁止优化
    QMAKE_CFLAGS_RELEASE   += -g               # 生成C调试信息
    QMAKE_CXXFLAGS_RELEASE -= -O2              # 取消C++优化
    QMAKE_CXXFLAGS_RELEASE += -O0              # 显示指定禁止优化
    QMAKE_CXXFLAGS_RELEASE += -g               # 生成C++调试信息
    QMAKE_LFLAGS_RELEASE   -= -Wl,-O1          # 取消Release模式链接器优化
    QMAKE_LFLAGS_RELEASE   += -g               # 链接器生成调试信息
#    message("GCC编译器Release关闭优化，生成调试信息使用"")
}
