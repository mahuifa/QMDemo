#---------------------------------------------------------------------------------------
# @功能：      Qt线程和并发编程示例
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2023-01-15 20:04:15
# @备注
#---------------------------------------------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += UseQThread1        # Qt通过【实例化】QThread，使用moveToThread将QObject子类槽函数在子线程中执行
SUBDIRS += UseQThread2        # Qt通过子类化，继承QThread，重写run实现子线程
SUBDIRS += UseQThreadPool     # Qt使用线程池QThreadPool示例
SUBDIRS += UseConcurrent      # Qt Concurent API使用示例
SUBDIRS += IOThread           # 在QT子线程中操作IO对象，包括QAbstractSocket、QFile、QSerialPort等
