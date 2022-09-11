#include "widget.h"
#include <QDebug>
#include <QtCharts>         // 导入QtCharts所有的头文件，也可以单独导入某一个

// 引入qchart命名空间（注意：如果是在ui中提升为QChartView，
// 则QT_CHARTS_USE_NAMESPACE一定要放在#include "ui_widget.h"前面，否则编译会失败）
QT_CHARTS_USE_NAMESPACE
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-面积图Demo - V%1").arg(APP_VERSION));
    initChart();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initChart()
{
    QLineSeries *series0 = new QLineSeries();    // 创建折线图对象，两条折线围成一个面
    QLineSeries *series1 = new QLineSeries();

    // 向两个折线图对象添加需要绘制的数据（注意每条线的点数需要 >= 2才会显示面积）
    *series0 << QPointF(1, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7) << QPointF(12, 6)
             << QPointF(16, 7) << QPointF(18, 5);
    *series1 << QPointF(1, 3) << QPointF(3, 4) << QPointF(7, 3) << QPointF(8, 2) << QPointF(12, 3)
             << QPointF(16, 4) << QPointF(18, 3);


    // 自定义渐变填充和轮廓的宽度
    QAreaSeries *series = new QAreaSeries(series0, series1);   // 创建面积图形式对象
    series->setName("面积");                                    // 设置当前面积图的名称
    series->setPen(QPen(QColor(0, 255, 255), 2));              // 设置绘制线条的颜色和线条粗细

    QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));    // 创建线性渐变
    gradient.setColorAt(0.0, QColor(0, 255, 0));
    gradient.setColorAt(1.0, QColor(0, 0, 255));
    gradient.setCoordinateMode(QGradient::ObjectMode);
    series->setBrush(gradient);                                // 设置绘制面积图的颜色（不设置也没影响）

#if 1
    QChart *chart = ui->chartView->chart();                    // 方法1：可以直接获取QChartView中的QChart
#else
    QChart *chart = new QChart();                              // 方法2：创建用于管理不同类型Series和其他其他图表相关对象（如图例和轴）的chart
    ui->chartView->setChart(chart);                            // 将包含series的QChart对象添加进ui中的chartView对象中
#endif
    chart->addSeries(series);
    chart->setTitle("这是面积图演示Demo的标题");
    chart->createDefaultAxes();                                // 创建1组默认的坐标轴（XY轴），如果没有添加series则会创建失败
    QList<QAbstractAxis*> axisX = chart->axes(Qt::Horizontal); // 获取创建的X轴
    QList<QAbstractAxis*> axisY = chart->axes(Qt::Vertical);   // 获取创建的X轴
    if(!axisX.isEmpty()) axisX.first()->setRange(0, 20);       // 设置X轴范围（设置前需要判断是否有X轴，避免数组越界）
    if(!axisY.isEmpty()) axisY.first()->setRange(0, 10);       // 设置Y轴范围

    ui->chartView->setRenderHint(QPainter::Antialiasing);     // 设置抗锯齿
}

