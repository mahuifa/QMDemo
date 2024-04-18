#---------------------------------------------------------------------------------------
# @功能：      OpenCV的一些常用Demo
# @编译器：     MSVC2017-64（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者      mhf
# @邮箱       1603291350@qq.com
# @时间       2022-07-04 16:10:30
# @备注
#---------------------------------------------------------------------------------------
TEMPLATE = subdirs

# CEF 库的路径， 如果路径为空则不编译当前工程
win32{
    opencvPath = E:/lib/opencv_MSVC/
}
unix:!macx{
}

# 判断库文件是否存在，如果不存在则终止编译
isEmpty($$opencvPath){
    exists($$opencvPath){
        SUBDIRS += VideoPlay
    }else{
        message(<<<<<<<<<<<<<<<< [OpenCV dll] The file path does not exist >>>>>>>>>>>>>>>>>>)
    }
}else{
    message(<<<<<<<<<<<<<<<< [OpenCV dll] The file path is NULL >>>>>>>>>>>>>>>>>>)
}

