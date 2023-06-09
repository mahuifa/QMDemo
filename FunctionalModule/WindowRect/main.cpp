#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    Widget w;
    w.show();
    return a.exec();
}
