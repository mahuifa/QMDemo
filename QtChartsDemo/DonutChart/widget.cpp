#include "widget.h"
#include <QPieSeries>
QT_CHARTS_USE_NAMESPACE   // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-圆环图Demo - V%1").arg(APP_VERSION));
    initChart();
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 初始化绘制圆环图，设置了图表样式，并且将一个圆环切片分离，显示当前切片的标签
 */
void Widget::initChart()
{
    QPieSeries* series = new QPieSeries();    // 创建一个圆环图对象
    series->setHoleSize(0.45);                // 设置圆环孔径大小0.0 ~1.0
    series->append("蛋白质占比 9.2%", 9.2);   // 将具有指定值和标签的单个切片附加到series
    series->append("其它 23.8%", 23.8);
    series->append("碳水化合物 51.4%", 51.4);
    QPieSlice* slice = series->append("脂肪占比 15.6%", 15.6);   // 设置一个圆环切片，并返回这个切片对象
    slice->setExploded();                                        // 将当前切片分离
    slice->setLabelVisible();                                    // 显示当前切片的标签，默认是不显示的

    ui->chartView->setRenderHint(QPainter::Antialiasing);               // 设置抗锯齿
    ui->chartView->chart()->setTitle("圆环图标题");                     // 设置图表标题
    ui->chartView->chart()->addSeries(series);                          // 将创建的圆环对象添加进chart中
    ui->chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);   // 设置图表的样式
    ui->chartView->chart()->legend()->setAlignment(Qt::AlignBottom);    // 指定图例显示在图表底部
    ui->chartView->chart()->legend()->setFont(QFont("Arial", 12));      // 设置图例的字体
}
