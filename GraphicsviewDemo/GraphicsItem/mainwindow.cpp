#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextFrameFormat>
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
    QPushButton* but9 = new QPushButton("窗口");
    QPushButton* but_clear = new QPushButton("清除");

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
    but_clear->setCheckable(true);

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
    m_butGroup.addButton(but_clear);

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
    ui->toolBar->addWidget(but_clear);

    // 绑定信号槽
    connect(but1, &QPushButton::clicked, this, &MainWindow::drawLine);
    connect(but2, &QPushButton::clicked, this, &MainWindow::drawRect);
    connect(but3, &QPushButton::clicked, this, &MainWindow::drawEllipse);
    connect(but4, &QPushButton::clicked, this, &MainWindow::drawPolygon);
    connect(but5, &QPushButton::clicked, this, &MainWindow::drawSimpleText);
    connect(but6, &QPushButton::clicked, this, &MainWindow::drawText);
    connect(but7, &QPushButton::clicked, this, &MainWindow::drawPixmap);
    connect(but8, &QPushButton::clicked, this, &MainWindow::drawPath);
    connect(but9, &QPushButton::clicked, this, &MainWindow::addWidget);
    connect(but_clear, &QPushButton::clicked, this, &MainWindow::clear);
}

/**
 * @brief  添加直线图元
 */
void MainWindow::drawLine()
{
    QLineF line(0, 0, 300, 0);
    QPen pen(Qt::red, 5);

#if 0              // 方式一，使用复杂一些，但更加灵活
    QGraphicsLineItem* item = new QGraphicsLineItem();
    item->setLine(line);
    item->setPen(pen);
    m_scene.addItem(item);
#else              // 方式二  这种方式将创建QGraphicsLineItem的步骤封装到函数内部了，使用简单
    m_scene.addLine(line, pen);
#endif
}

/**
 * @brief 添加矩形图元
 */
void MainWindow::drawRect()
{
    QRectF rectf(0, 0, 200, 200);
    QPen pen(Qt::red, 5);
    QBrush brush(Qt::cyan);
#if 0        // 方式一
    QGraphicsRectItem* item = new QGraphicsRectItem();
    item->setRect(rectf);
    item->setPen(pen);
    item->setBrush(brush);
    m_scene.addItem(item);
#else       // 方式二
    m_scene.addRect(rectf, pen, brush);
#endif
}

/**
 * @brief 添加椭圆图元
 */
void MainWindow::drawEllipse()
{
    QRectF rectf(0, 0, 200, 200);
    QPen pen(Qt::red, 5);
    QBrush brush(Qt::cyan);
#if 0        // 方式一
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem();
    item->setRect(rectf);
    item->setPen(pen);
    item->setBrush(brush);
    m_scene.addItem(item);
#else       // 方式二
    m_scene.addEllipse(rectf, pen, brush);
#endif
}

/**
 * @brief 添加多边形图元
 */
void MainWindow::drawPolygon()
{
    QPolygonF polygonF;
    polygonF << QPointF(0, 0) << QPointF(100, 50) << QPointF(150, 200);
    QPen pen(Qt::red, 5);
    QBrush brush(Qt::cyan);
#if 0        // 方式一
    QGraphicsPolygonItem* item = new QGraphicsPolygonItem();
    item->setPolygon(polygonF);
    item->setPen(pen);
    item->setBrush(brush);
    m_scene.addItem(item);
#else       // 方式二
    m_scene.addPolygon(polygonF, pen, brush);
#endif
}

/**
 * @brief 添加简单文本图元
 */
void MainWindow::drawSimpleText()
{
    QFont font("宋体", 30, QFont::Bold);
#if 1        // 方式一
    QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem();
    item->setText("简单文本");
    item->setFont(font);
    m_scene.addItem(item);
#else          // 方式二
    m_scene.addSimpleText("简单文本", font);
#endif
}

/**
 * @brief 添加文本图元，支持富文本，功能更加强大
 */
void MainWindow::drawText()
{
    QFont font("宋体", 30, QFont::Bold);
#if 1        // 方式一
    // 显示纯文本
    QGraphicsTextItem *item1 = new QGraphicsTextItem();
    item1->setPlainText("文本");
    item1->setFont(font);
    item1->setDefaultTextColor(Qt::red);
    m_scene.addItem(item1);

    // 显示HTML文本，可通过HTML设置，功能更加强大
    QGraphicsTextItem *item2 = new QGraphicsTextItem();
    item2->setHtml("<h1>Html文本</h1>");
    item2->setPos(0, 100);
    m_scene.addItem(item2);

    // 富文本
    QGraphicsTextItem *item3 = new QGraphicsTextItem();
    QTextDocument* document = new QTextDocument();
    document->setHtml("<p><span style=\" font-family:'SimSun'; font-size:26pt; color:#00aaff;\">富文本</span></p>");
    item3->setDocument(document);
    item3->setPos(0, 200);
    m_scene.addItem(item3);
#else          // 方式二
    m_scene.addText("文本", font);
#endif
}

/**
 * @brief 添加图片图元
 */
void MainWindow::drawPixmap()
{
    QPixmap pixmap(":/1.png");
#if 0
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
    item->setPixmap(pixmap);
    m_scene.addItem(item);
#else
    m_scene.addPixmap(pixmap);
#endif
}

/**
 * @brief 添加绘图路径图元
 */
void MainWindow::drawPath()
{
    QPen pen(Qt::red, 5);
    QBrush brush(Qt::cyan);
    QPainterPath path;
    // 椭圆（圆形）
    path.addEllipse(150, 10, 100, 100);
    // 矩形
    path.addRect(300, 10, 100, 60);
    // 圆形区域
    path.addRegion(QRegion(450, 10, 40, 50, QRegion::Ellipse));
    // 圆角矩形
    path.addRoundRect(QRect(20, 150, 100, 100), 20, 20);

#if 0          // 方式一
    QGraphicsPathItem* item = new QGraphicsPathItem();
    item->setPath(path);
    m_scene.addItem(item);
#else           // 方式二
    m_scene.addPath(path, pen, brush);
#endif
}

/**
 * @brief 嵌入窗口部件
 */
void MainWindow::addWidget()
{
    QPushButton* but = new QPushButton("button");
#if 0
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
    proxy->setWidget(but);
    m_scene.addItem(proxy);
#else
    m_scene.addWidget(but);
#endif
}

/**
 * @brief 移除和删除所有项目
 */
void MainWindow::clear()
{
    m_scene.clear();
}

