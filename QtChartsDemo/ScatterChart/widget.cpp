#include "widget.h"
#include <QScatterSeries>
#include <QRandomGenerator>
#include <qmath.h>
QT_CHARTS_USE_NAMESPACE    // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-散点图Demo - V%1").arg(APP_VERSION));
    initChart();
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 初始化绘制图表，在initChart没有添加散点，在点击按键后再绘制散点图
 */
void Widget::initChart()
{
    ui->chartView->setRenderHint(QPainter::Antialiasing);                              // 设置抗锯齿
    ui->chartView->chart()->setTitle("散点图标题");                                      // 设置图表标题
    ui->chartView->chart()->setDropShadowEnabled(false);                               // 设置不启用背景阴影效果。
    ui->chartView->chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);  // 在图例中显示点的形状样式
    ui->chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);                  // 设置表的样式
}

/**
 * @brief 在图表中添加圆点样式散点
 */
void Widget::on_pushButton_clicked()
{
    QScatterSeries* series = new QScatterSeries();              // 创建一个散点数据集对象
    series->setName("圆点样式");
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);  // 设置绘制的散点的样式为圆
    series->setMarkerSize(10);                                  // 设置绘制的点的大小

    for(int i = 0; i < 20; i++)    // 添加10个任意点
    {
        series->append(i, QRandomGenerator::global()->bounded(10, 50));
    }
    ui->chartView->chart()->addSeries(series);       // 将创建的series添加经chart中
    ui->chartView->chart()->createDefaultAxes();     // 新添加series后，调用这个函数根据添加的series自动生成对于类型的XY轴，会删除已有的轴再生成（推荐使用这种方法，尽量不要自己添加轴）
}

/**
 * @brief 在图表中添加方形样式散点
 */
void Widget::on_pushButton_2_clicked()
{
    QScatterSeries* series = new QScatterSeries();                 // 创建一个散点数据集对象
    series->setName("方形样式");
    series->setMarkerShape(QScatterSeries::MarkerShapeRectangle);  // 设置绘制的散点的样式为矩形
    series->setMarkerSize(10);                                     // 设置绘制的点的大小

    for(int i = 0; i < 20; i++)    // 添加10个任意点
    {
        *series << QPointF(i, QRandomGenerator::global()->bounded(10, 50)); // 换一种添加数据的方式
    }
    ui->chartView->chart()->addSeries(series);       // 将创建的series添加经chart中
    ui->chartView->chart()->createDefaultAxes();     // 根据已有series生成对于类型的坐标轴，并将轴附着到所有的series
}

/**
 * @brief 在图表中添加五角星样式散点，由于内置的点类型中有圆形和矩形，想要五角星就只能自己画,
 *        可以使用QPainter画或者贴图，所以既然可以画五角星，那当然也可以画其它的图案啦，比如：超级玛丽
 */
void Widget::on_pushButton_3_clicked()
{
    int size = 30;                                                 // 五角星大小为长宽30个像素
    QScatterSeries* series = new QScatterSeries();                 // 创建一个散点数据集对象
    series->setName("五角星样式");
    series->setMarkerShape(QScatterSeries::MarkerShapeRectangle);  // 设置绘制的散点的样式为矩形
    series->setMarkerSize(size);                                   // 设置绘制的点的大小为长宽30个像素

    for(int i = 0; i < 20; i++)    // 添加10个任意点
    {
        series->append(i, QRandomGenerator::global()->bounded(10, 50));
    }

    // 再一个透明图层中画一个五角星图案
    QPainterPath starPath;
    starPath.moveTo(size - 2, size / 2);              // 移动画五角星线段的起始位置
    for(int i = 1; i < 5; i++)
    {
        qreal half = (size / 2) - 1;
        qreal x = half + half * qCos(0.8 * i * M_PI);      // qCos函数需要使用到qmath.h
        qreal y = half + 1 + half * qSin(0.8 * i * M_PI);
        starPath.lineTo(x, y);                       // 添加五角星的绘制路径
    }
    starPath.closeSubpath();                         // 关闭当前子路径
    QImage image(size, size, QImage::Format_ARGB32); // 创建一个30*30的空白图层，将五角星画到图层上
    image.fill(Qt::transparent);                     // 用给定的像素值填充整个图像（设置透明图层）

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);   // 设置抗锯齿
    painter.setPen(QColor(0, 255, 255));
    painter.setBrush(QColor(255, 255, 0));
    painter.drawPath(starPath);

    series->setBrush(image);                         // 将画有五角星的图层设置为点的样式
    series->setPen(QColor(Qt::transparent));         // 默认绘制点是有一个白色的边框的，所以画五角星是就要将边框设透明

    ui->chartView->chart()->addSeries(series);       // 将创建的series添加经chart中
    ui->chartView->chart()->createDefaultAxes();
}

