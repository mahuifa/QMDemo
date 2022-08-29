#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPushButton>
#include <qgraphicsitem.h>
#include <qtoolbutton.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initToolBar();

    ui->graphicsView->setScene(&m_scene);          // 添加绘图场景
}

MainWindow::~MainWindow()
{
    delete ui;
}

#if 0
enum ItemType{
    Line,                // 直线
    Rect,                // 矩形
    Ellipse,             // 椭圆
    Polygon,             // 多边形
    SimpleText,          // 简单文本
    Text,                // 文本
    Pixmap,              // 图片
    Path                 // 路径
};
#endif

/**
 * @brief  初始化工具栏
 */
void MainWindow::initToolBar()
{
    QPushButton* but1 = new QPushButton("直线");
    QPushButton* but2 = new QPushButton("矩形");
    QPushButton* but3 = new QPushButton("椭圆");
    QPushButton* but4 = new QPushButton("多边形");
    QPushButton* but5 = new QPushButton("简单文本");
    QPushButton* but6 = new QPushButton("文本");
    QPushButton* but7 = new QPushButton("图片");
    QPushButton* but8 = new QPushButton("路径");
    QPushButton* but9 = new QPushButton("清除");

    // 按键设置可选
    but1->setCheckable(true);
    but2->setCheckable(true);
    but3->setCheckable(true);
    but4->setCheckable(true);
    but5->setCheckable(true);
    but6->setCheckable(true);
    but7->setCheckable(true);
    but8->setCheckable(true);
    but9->setCheckable(true);

    // 添加进按键组，默认是单选模式
    m_butGroup.addButton(but1);
    m_butGroup.addButton(but2);
    m_butGroup.addButton(but3);
    m_butGroup.addButton(but4);
    m_butGroup.addButton(but5);
    m_butGroup.addButton(but6);
    m_butGroup.addButton(but7);
    m_butGroup.addButton(but8);
    m_butGroup.addButton(but9);

    // 添加进工具栏
    ui->toolBar->addWidget(but1);
    ui->toolBar->addWidget(but2);
    ui->toolBar->addWidget(but3);
    ui->toolBar->addWidget(but4);
    ui->toolBar->addWidget(but5);
    ui->toolBar->addWidget(but6);
    ui->toolBar->addWidget(but7);
    ui->toolBar->addWidget(but8);
    ui->toolBar->addWidget(but9);

    // 绑定信号槽
    connect(but1, &QPushButton::clicked, this, &MainWindow::drawLine);
    connect(but2, &QPushButton::clicked, this, &MainWindow::drawRect);
    connect(but3, &QPushButton::clicked, this, &MainWindow::drawEllipse);
    connect(but4, &QPushButton::clicked, this, &MainWindow::drawPolygon);
    connect(but5, &QPushButton::clicked, this, &MainWindow::drawSimpleText);
    connect(but6, &QPushButton::clicked, this, &MainWindow::drawText);
    connect(but7, &QPushButton::clicked, this, &MainWindow::drawPixmap);
    connect(but8, &QPushButton::clicked, this, &MainWindow::drawPath);
    connect(but9, &QPushButton::clicked, this, &MainWindow::clear);
}

/**
 * @brief  添加直线图元
 */
void MainWindow::drawLine()
{
    QLineF line(0, 0, 300, 0);
    QPen pen(Qt::red, 5);

#if 0              // 方式一
    QGraphicsLineItem* item = new QGraphicsLineItem();
    item->setLine(line);
    item->setPen(pen);
    m_scene.addItem(item);
#else              // 方式二  这种方式将创建QGraphicsLineItem的步骤封装到函数内部了
    m_scene.addLine(line, pen);
#endif
}

void MainWindow::drawRect()
{

}

void MainWindow::drawEllipse()
{

}

void MainWindow::drawPolygon()
{

}

void MainWindow::drawSimpleText()
{

}

void MainWindow::drawText()
{

}

void MainWindow::drawPixmap()
{

}

void MainWindow::drawPath()
{

}

void MainWindow::clear()
{

}

