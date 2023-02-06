#---------------------------------------------------------------------------------------
# @功能：       在Qt中嵌入CEF实现Qt-Web混合开发
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2023-02-05 12:05:07
# @备注       下载当前源码后需要自己修改CEF库路径CefPath，否则不会编译，子工程也需要修改
#---------------------------------------------------------------------------------------
TEMPLATE = subdirs

# CEF 库的路径， 如果路径为空则不编译当前工程
win32{
    CefPath = E:/lib/QCefView
}
unix:!macx{
}

# 判断库文件是否存在，如果不存在则终止编译
isEmpty($$CefPath){
    exists($$CefPath){
        SUBDIRS += LoadWeb     # Qt使用cef加载网页简单示例
    }else{
        message(<<<<<<<<<<<<<<<< [CEF dll] The file path does not exist >>>>>>>>>>>>>>>>>>)
    }
}else{
    message(<<<<<<<<<<<<<<<< [CEF dll] The file path is NULL >>>>>>>>>>>>>>>>>>)
}
