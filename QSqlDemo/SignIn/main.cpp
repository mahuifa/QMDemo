#include "widget.h"
#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    // 打开登录界面
    Dialog signIn;
    signIn.exec();

    if(!signIn.loggedIn())   // 登录失败直接退出
    {
        return 0;
    }
    // 登录成功打开主界面
    Widget w;
    w.show();
    return a.exec();
}
