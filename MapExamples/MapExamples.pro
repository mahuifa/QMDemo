#---------------------------------------------------------------------------------------
# @功能：      与地图相关的示例代码
# @编译器：     Desktop Qt 5.14.2 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.14.2/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2024-03-29 14:45:06
# @备注
#---------------------------------------------------------------------------------------

TEMPLATE = subdirs

SUBDIRS += MapDownload       # 简单的瓦片地图下载示例
SUBDIRS += MapView           # Qt使用QGraphicsView显示瓦片地图简单示例
SUBDIRS += MapView2          # Qt以绝对像素坐标显示离线瓦片地图
SUBDIRS += MapView3          # Qt以绝对像素坐标显示在线瓦片地图
