#include <QApplication>
#include <QDesktopServices>
#include <qdir.h>
#include <qurl.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /**
     * 使用合适的程序打开url
     * QDesktopServices需要使用QApplication才可以执行，
     * 否则会报错【QDesktopServices::openUrl: Application is not a GUI application】
     */
    qDebug() << QDesktopServices::openUrl(QDir("./html/chatClient.html").absolutePath());

    return a.exec();
}
