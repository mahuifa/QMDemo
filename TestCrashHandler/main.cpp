#include "widget.h"
#include "crashhandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CrashHandler::initCrashHandler();     // 注册崩溃处理功能
    Widget w;
    w.show();
    return a.exec();
}
