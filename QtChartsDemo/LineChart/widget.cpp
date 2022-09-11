#include "widget.h"
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE    // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-折线图Demo - V%1").arg(APP_VERSION));
    initChart();
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 初始化绘制图表
 */
void Widget::initChart()
{
    QLineSeries* series = new QLineSeries();         // 创建一个折线图series
    series->setName("线段名称");

#if 1
    // 添加数据方法1
    series->append(0, 5);
    series->append(1, 4);
    series->append(3, 8);
    // 添加数据方法2
    *series << QPointF(4, 10)  << QPointF(5, 2)  << QPointF(6, 5);
#else
    // 添加数据方法3，一次更新所有数据，速度比较快
    QList<QPointF> points;
    for(int i = 0; i < 20; i++)
    {
        points.append(QPointF(i, i %5));
    }
    series->replace(points);
#endif
    QChart* chart = ui->chartView->chart();     // 获取QchartView内置的chart
    chart->addSeries(series);                   // 将创建好的折线图series添加进chart中
    chart->createDefaultAxes();                 // 基于已添加到图表中的series为图表创建轴。以前添加到图表中的任何轴都将被删除。
    chart->setTitle("折线图图表标题");            // 设置标题

    ui->chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
}

