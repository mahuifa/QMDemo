#include "widget.h"
#include "ui_widget.h"

#include <qdebug.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-盒须图（箱型图）Demo - V%1").arg(APP_VERSION));
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
    QBoxPlotSeries* series = new QBoxPlotSeries();             // 创建一个用于绘制盒须图的数据集对象
    series->setName("XXX公司");
    series->append(getData());                                 // 读取资源文件中的数据并添加进series

    QChart* chart = ui->chartView->chart();
    chart->addSeries(series);                                  // 将需要绘制的数据集（series）添加进char中
    chart->setTitle("盒须图标题");                               // 设置图表标题
    chart->setAnimationOptions(QChart::SeriesAnimations);      // 设置图表显示的动画（对性能有影响）
    chart->createDefaultAxes();                                // 根据已有的series生成对应类型的XY轴

    QAbstractAxis* axisY = chart->axes(Qt::Vertical).first();  // 获取Y轴
    axisY->setRange(15, 35);                                   // 设置Y轴范围，让图表更容易观看（Qt的示例中这里有bug，设置成了X轴）
    chart->legend()->setVisible(true);                         // 显示图例
    chart->legend()->setAlignment(Qt::AlignBottom);            // 设置图例显示位置

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);      // 设置抗锯齿
}

/**
 * @brief  读取资源文件，返回用于绘制的盒须图数据
 * @return 返回用于绘制的数据，如果count为0则没有可用数据
 */
QList<QBoxSet *> Widget::getData()
{
    QList<QBoxSet *> dataList;
    QList<qreal> sortedList;

    QFile file(":/acme.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))   // 文件打开失败
        return dataList;

    while (!file.atEnd())
    {
        QString line = file.readLine();
        if(line.isEmpty() || line.startsWith("#"))         // 如果数据无效则跳过
            continue;

        QStringList strList = line.split(" ", QString::SkipEmptyParts);  // 以空格拆分字符串

        sortedList.clear();
        for(int i = 1; i < strList.count(); i++)
        {
            sortedList.append(strList.at(i).toDouble());
        }

        std::sort(sortedList.begin(), sortedList.end());                 // 由于qt中提示qSort过时了，推荐使用std::sort
        int count = sortedList.count();

        QBoxSet* box = new QBoxSet();    // 创建盒须图的一个数据（由下限、下四分位数、中位数、上四分位数和上限 五个值组成）
        box->setLabel(strList.first());  // 设置标签
        box->setValue(QBoxSet::LowerExtreme, sortedList.first());                         // 下极限
        box->setValue(QBoxSet::UpperExtreme, sortedList.last());                          // 上极限
        box->setValue(QBoxSet::Median, findMedian(sortedList, 0, count));                 // 中位数
        box->setValue(QBoxSet::LowerQuartile, findMedian(sortedList, 0, count / 2));      // 下四分位数
        box->setValue(QBoxSet::UpperQuartile,
                      findMedian(sortedList, (count / 2) + (count % 2), count));          // 上四分位数,(count % 2)当数据个数为奇数时向上取整
        dataList.append(box);
    }

    return dataList;
}

/**
 * @brief              计算数组中的中位数，如果设置起始终止位置可用计算（下上四分位数、上四分位数）
 * @param sortedList   需要计算中位数的数组
 * @param begin        起始位置
 * @param end          中止位置
 * @return             中位数（下四分位数、上四分位数）
 */
qreal Widget::findMedian(QList<qreal> &sortedList, int begin, int end)
{
    int count = end - begin;
    if(count % 2)
    {
        return sortedList.at(count / 2 + begin);
    }
    else   // 如果需要计算的数是偶数，则取两个数的平均值
    {
        qreal right = sortedList.at(count / 2 + begin);
        qreal left = sortedList.at(count / 2 - 1 + begin);
        return (right + left) / 2.0;
    }
}
