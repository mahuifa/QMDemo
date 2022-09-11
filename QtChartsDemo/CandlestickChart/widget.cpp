#include "widget.h"
#include <QBarCategoryAxis>
#include <QCandlestickSeries>
#include <QDateTime>
#include <QValueAxis>
#include <qdebug.h>
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-蜡烛图Demo - V%1").arg(APP_VERSION));
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
    QCandlestickSeries* series = new QCandlestickSeries();   // 创建一个蜡烛图数据集对象，用于绘制蜡烛图
    series->setName("今日股票走势");                           // 设置名称
    series->setIncreasingColor(QColor(Qt::green));           // 设置增长的烛台颜色（当烛台的关闭值高于打开值时，烛台会增加）
    series->setDecreasingColor(QColor(Qt::red));             // 设置减少的烛台的颜色（当烛台的打开值高于关闭值时，烛台正在减小）

    const QList<QCandlestickSet *> dataList = getData();           // 获取用于绘制烛台的数据
    series->append(dataList);

    ui->chartView->chart()->addSeries(series);               // 将创建的烛台图数据集对象添加进chart
    ui->chartView->chart()->setTitle("烛台图标题");            // 设置图表标题
    ui->chartView->chart()->setAnimationOptions(QChart::SeriesAnimations);  // 设置图表动画效果（会影响性能）
    ui->chartView->chart()->createDefaultAxes();             // 根据已有的series生成对应类型的XY轴
    ui->chartView->chart()->legend()->setVisible(true);      // 显示图例
    ui->chartView->chart()->legend()->setAlignment(Qt::AlignBottom);  // 图例显示在图表底部
    ui->chartView->setRenderHint(QPainter::Antialiasing);    // 设置抗锯齿

    // 获取Y轴，并将类型转换为QValueAxis
    QValueAxis* axisY = qobject_cast<QValueAxis*>(ui->chartView->chart()->axes(Qt::Vertical).at(0));
    axisY->applyNiceNumbers();                               // 调整Y轴范围，使可用显示所有数据，便于观看

    // 获取X轴，并将类型转换为QBarCategoryAxis
    QBarCategoryAxis* axisX = qobject_cast<QBarCategoryAxis*>(ui->chartView->chart()->axes(Qt::Horizontal).at(0));
    QStringList categories;
    for(auto set : dataList)
    {
        QString strTime = QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString("dd");
        categories.append(strTime);
    }
    axisX->setCategories(categories);                       // 以数据的时间设置X轴的类别（不能重复）
}

/**
 * @brief   读取文件中用来绘制图表的数据
 * @return  返回所有需要绘制的数据，如果count等于0则表示没有数据
 */
QList<QCandlestickSet *> Widget::getData()
{
    QList<QCandlestickSet *> dataList;
    QFile file(":/acme.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))  // 打开数据文件并判断是否打开失败
    {
        qWarning() << "打开文件失败！";
        return dataList;
    }
    while (!file.atEnd())
    {
        QString line = file.readLine();   // 读取一行数据
        if(line.startsWith("#")           // 如果字符串以#开头，则返回true；否则返回false。
        || line.isEmpty())                // 判断是否读取到空数据
            continue;

        QStringList strList = line.split(" ", QString::SkipEmptyParts);  // 以空格拆分字符串
        if(strList.count() != 5)          // 判断数据是否完整有效
            continue;

        const qreal timestamp = strList.at(0).toDouble();
        const qreal open = strList.at(1).toDouble();
        const qreal high = strList.at(2).toDouble();
        const qreal low = strList.at(3).toDouble();
        const qreal close = strList.at(4).toDouble();

        // 创建单个烛台数据对象，有打开、高、低、关闭和时间戳五个值组成
        QCandlestickSet* candlestickSet = new QCandlestickSet();
        candlestickSet->setTimestamp(timestamp);
        candlestickSet->setOpen(open);
        candlestickSet->setHigh(high);
        candlestickSet->setLow(low);
        candlestickSet->setClose(close);

        dataList.append(candlestickSet);
    }
    return dataList;
}

