#---------------------------------------------------------------------------------------
# @功能：       Qt Concurent API使用示例
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2023-02-23 13:58:59
# @备注
#---------------------------------------------------------------------------------------
requires(qtHaveModule(concurrent))  # 如果条件为 false，qmake 在构建时会跳过此项目

TEMPLATE = subdirs

SUBDIRS += RunFunction
