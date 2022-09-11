#include "widget.h"
#include <QPolarChart>
#include <QScatterSeries>
#include <qdebug.h>
#include <qvalueaxis.h>
QT_CHARTS_USE_NAMESPACE    // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-极坐标图Demo - V%1").arg(APP_VERSION));
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
    QScatterSeries* series = new QScatterSeries();     // 创建一个散点绘图数据集对象
    const qreal angularMax = 100;    // 最大角度
    series->setName("散点");
    for(int i = 0; i < angularMax; i += 10)
    {
        series->append(i, i + 100);                   // 向series中添加数据，X：角度 Y：到圆心距离
    }

    QPolarChart* chart = new QPolarChart();           // 创建一个极坐标系图表
    chart->legend()->setVisible(false);               // 隐藏图例
    chart->addSeries(series);                         // 将创建的series添加进图表中
    chart->createDefaultAxes();                       // 根据已有的series生成默认的坐标轴
    ui->chartView->setChart(chart);
}
