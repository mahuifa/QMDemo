#include "widget.h"
#include "ui_widget.h"
#include "test1.h"
#include "test2.h"
#include "test3.h"
#include <QProcess>
#include <QDebug>
#include <qdir.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");

    this->setWindowTitle(QString("QXlsx操作Excel 示例程序 - V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_but_show_clicked()
{
    InterFace* interFace = static_cast<InterFace*>(ui->tabWidget->currentWidget());  // 获取当前tabwidget中的widget并转换为接口对象
    bool ret = QProcess::startDetached("D:/WPS Office/ksolaunch.exe",
                                       QStringList() << QDir::currentPath() + "/" + interFace->getExcelName());  // 打开当前示例用到excel文件
    if(!ret)
    {
        qWarning() << "打开Excel失败，请注意wps路径是否存在，或者替换其它程序打开excel";
    }
}
