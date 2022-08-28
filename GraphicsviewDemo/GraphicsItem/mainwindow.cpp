#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPushButton>
#include <qtoolbutton.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief  初始化工具栏
 */
void MainWindow::initToolBar()
{
    QPushButton* but1 = new QPushButton("直线");
    QPushButton* but2 = new QPushButton("矩形");

    // 按键设置可选
    but1->setCheckable(true);
    but2->setCheckable(true);

    // 添加进按键组，默认是单选模式
    m_butGroup.addButton(but1);
    m_butGroup.addButton(but2);

    // 添加进工具栏
    ui->toolBar->addWidget(but1);
    ui->toolBar->addWidget(but2);
}

