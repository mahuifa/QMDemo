#include "widget.h"

#include <QApplication>
#include "logsavetxt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogSaveTxt::initLog();           // 初始化日志保存功能，如果想保存程序启动的所有日志就加上这一行，如果通过提升为LogWidgetText则在这个控件初始化之前的日志不会保存
    Widget w;
    w.show();

    return a.exec();
}
