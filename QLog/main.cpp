#include "widget.h"

#include <QApplication>
#include "QLog/loginput.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogInput::getInstance();
    Widget w;
    w.show();

    return a.exec();
}
