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

SUBDIRS += RunFunction      # QtConcurrent::run使用示例，在另一个线程中运行一个函数。
SUBDIRS += Map              # QtConcurrent::map使用示例，可以在多线程环境下高效地处理大量数据，并且可以返回一个QFuture对象，可以使用它来跟踪数据处理的进度。
SUBDIRS += Mapped           # QtConcurrent::mapped使用示例，与 map（） 类似，不同之处在于它返回了一个包含返回值的新容器。
SUBDIRS += MappedReduced    # QtConcurrent::mappedReduced使用示例，与mapped()类型，相当于将mapped()的结果放入到一个单线程函数中进行计算
