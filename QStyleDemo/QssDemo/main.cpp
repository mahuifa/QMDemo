#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);                    // 设置使用utf-8字符编码，否则qss有中文则无法生效

    MainWindow w;
    w.show();
    return a.exec();
}
