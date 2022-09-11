#include "widget.h"
#include <QPieSeries>
QT_CHARTS_USE_NAMESPACE    // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-饼图Demo - V%1").arg(APP_VERSION));
    initChart();
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 初始化绘制饼图，饼图和圆环图的绘制方式基本一样，只是
 *        不通过setHoleSize设置大于孔径，默认为0，就是没有孔径
 */
void Widget::initChart()
{
    QPieSeries* series = new QPieSeries();            // 创建一个饼图对象（设置孔径就是圆环）
    series->append("星期一", 1);                       // 添加饼图切片
    series->append("星期二", 2);
    series->append("星期三", 3);
    series->append("星期四", 4);
    series->append("星期五", 5);

    QPieSlice* slice = series->slices().at(1);       // 获取饼图中某一个切片（在绘制圆环图Demo中是通过appent函数获取，这里换一种方式）
    slice->setExploded();                            // 将切片分离饼图
    slice->setLabelVisible();                        // 显示当前切片的标签
    slice->setPen(QPen(Qt::darkGreen, 2));           // 设置画笔颜色和粗细
    slice->setBrush(Qt::green);                      // 设置切片背景色

    QChart* chart = ui->chartView->chart();           // 获取QChartView中默认的QChart
    chart->addSeries(series);                        // 将创建好的饼图对象添加进QChart
    chart->setTitle("饼图标题");                      // 设置图表标题
    ui->chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
}
