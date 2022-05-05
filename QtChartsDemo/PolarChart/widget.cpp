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

    this->setWindowTitle("QtCharts绘图-极坐标图Demo");
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
    const qreal angularMax = 100;    // 最大角度
    const qreal radialMax = 100;     // 最大径向
    QScatterSeries* series = new QScatterSeries();
    series->setName("散点");
    for(int i = 0; i < angularMax; i += 10)
    {
        series->append(i, (i / radialMax) * radialMax + 8.0);
    }

    QPolarChart* chart = new QPolarChart();
    chart->legend()->setVisible(false);
    chart->addSeries(series);
    ui->chartView->setChart(chart);

    QValueAxis* angularAxis = new QValueAxis();

}
