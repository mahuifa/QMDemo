#include "test3.h"
#include "ui_test3.h"

#include <QDir>
#include <qprocess.h>
#include <QDebug>

Test3::Test3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Test3)
{
    ui->setupUi(this);
}

Test3::~Test3()
{
    delete ui;
}


/**
 * @brief  通过调用WPS打开当前路径下的Excel文件，如果打开失败需要替换自己的wps的安装路径
 */
void Test3::on_but_show_clicked()
{
    bool ret = QProcess::startDetached("D:/WPS Office/ksolaunch.exe", QStringList() << QDir::currentPath() + "/1.xlsx");
    if(!ret)
    {
        qWarning() << "打开Excel失败，请注意wps路径是否存在，或者替换其它程序打开excel";
    }
}
