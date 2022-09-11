#include "widget.h"
#include <QSplineSeries>
QT_CHARTS_USE_NAMESPACE    // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-样条曲线图Demo - V%1").arg(APP_VERSION));
    initChart();
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 初始化绘制曲线图
 */
void Widget::initChart()
{
    QSplineSeries* series = new QSplineSeries();   // 创建一个样条曲线对象
    series->setName("曲线");

#if 1
    // 添加数据方式1
    series->append(0, 10);
    series->append(1, 1);
    series->append(2, 4);
    series->append(3, 7);
    series->append(4, 13);
    // 添加数据方式2
    *series << QPointF(5, 3)<< QPointF(6, 6)<< QPointF(7, 13)<< QPointF(8, 5);
#else  // 添加数据方式3，一次性更新所有数据
    QList<QPointF> points;
    for(int i = 0; i < 20; i++)
    {
        points.append(QPointF(i, i %7));
    }
    series->replace(points);
#endif

    QChart* chart = ui->chartView->chart();    // 获取一个chart用于管理不同类型的series和其他图表相关对象
    chart->legend()->hide();                   // 隐藏图例
    chart->addSeries(series);                  // 添加创建好的曲线图对象
    chart->setTitle("曲线图图表标题");           // 设置标题
    chart->createDefaultAxes();                // 基于已添加到图表中的series为图表创建轴。以前添加到图表中的任何轴都将被删除。
    chart->axes(Qt::Vertical).first()->setRange(0, 20);  // 设置Y轴的范围

    ui->chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
}
