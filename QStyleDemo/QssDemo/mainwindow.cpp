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

