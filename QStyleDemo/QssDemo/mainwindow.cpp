#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
    initStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->mdiArea->setBackground(Qt::NoBrush);           // 如果不设置则QMdiArea的qss样式不会生效
    ui->checkBox->setTristate(true);                   // 开启QCheckBox不确定态
    ui->checkBox->setGeometry(0, 0, 20, 20);
}

/**
 * @brief 加载qss文件
 */
void MainWindow::initStyle()
{
    QFile file(":/style.css");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);

        QString strQss;
        while (!stream.atEnd())
        {
            strQss.append(stream.readLine());
        }
        qApp->setStyleSheet(strQss);               // 设置整个程序的样式表而不是当前窗口
    }
    else
    {
        qWarning() << "打开qss文件失败！";
    }
}

/**
 * @brief       打印QCheckBox选中、未选中、不确定三态
 * @param arg1
 */
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    qDebug() << arg1;
}
