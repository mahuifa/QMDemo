#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);  // 在支持的平台上启用Qt中的高DPI缩放
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
