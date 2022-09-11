#include "widget.h"
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

    this->setWindowTitle(QString("QtCharts绘图-条形图Demo - V%1").arg(APP_VERSION));

    initChart();
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 初始化绘制条形图
 */
void Widget::initChart()
{
    // QBarSet 类表示条形图中的一组条形
    QBarSet* set0 = new QBarSet("C++");
    QBarSet* set1 = new QBarSet("java");
    QBarSet* set2 = new QBarSet("python");
    QBarSet* set3 = new QBarSet("html");
    QBarSet* set4 = new QBarSet("css");

    // 为每一个条形添加数据
    *set0 << 1 << 2 << 3 << 4 << 5 << 6;
    *set1 << 5 << 0 << 0 << 4 << 0 << 7;
    *set2 << 4 << 5 << 8 << 13 << 8 << 5;
    *set3 << 5 << 6 << 7 << 3 << 4 << 5;
    *set4 << 9 << 7 << 5 << 3 << 1 << 2;

    // QBarSeries 类将一系列数据显示为按类别分组的垂直条。
    QBarSeries* series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);

    // QChart 类管理图表系列(series)、图例(legends)和轴(axes)。
    QChart* chart = ui->chartView->chart();                // 获取QChartView自带的chart
    chart->addSeries(series);                              // 将创建好的条形图series添加进chart中
    chart->setTitle("关于学习编程时间的条形图");               // 设置标题
    chart->setAnimationOptions(QChart::SeriesAnimations);  // 设置图表变化时的动画效果

    QStringList categories;                                // X轴分类，一般与QBarSet中添加的数据个数相同，如果少了则会显示不全,多了不影响，但是不能重复
    categories << "星期一" << "星期二"  << "星期三"
               << "星期四"  << "星期五"  << "星期六" << "星期天";  // 星期天当然是休息了...
    QBarCategoryAxis* axisX = new QBarCategoryAxis();      // QBarCategoryAxis类向图表的轴添加类别。
    axisX->setTitleText("X轴名称是：星期几");                // 设置X轴标题
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);                // 将X轴放在图表的底部
    series->attachAxis(axisX);                             // 将axis指定的轴附着到series。

    QValueAxis* axisY = new QValueAxis();     // 创建Y轴
    axisY->setRange(0, 24);                   // 设置Y轴范围（学习啊，当然是24小时喽）
    axisY->setTitleText("Y轴当然是：学习时间");  // 设置Y轴标题
    chart->addAxis(axisY, Qt::AlignLeft);     // Y轴左对齐
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);             // 设置图例是否可见（默认是可见的）
    chart->legend()->setAlignment(Qt::AlignRight); // 设置图例显示的位置

    ui->chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿（不过在条形图中不设置也没关系）
}

