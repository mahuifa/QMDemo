#include "test4.h"
#include "ui_test4.h"

#include <qdir.h>
#include <qprocess.h>
#include <QDebug>
#include <xlsxdocument.h>
#include <xlsxchart.h>
QXLSX_USE_NAMESPACE

#define EXCEL_NAME "chart.xlsx"         // 本Demo使用的Excel文件名

Test4::Test4(QWidget *parent) :
    InterFace(parent),
    ui(new Ui::Test4)
{
    ui->setupUi(this);
    this->setWindowTitle("QXlsx在工作表中插入所有支持的图表、插入图表Sheet");
    this->setToolTip(this->windowTitle());
}

Test4::~Test4()
{
    delete ui;
}

QString Test4::getExcelName()
{
    return EXCEL_NAME;
}

/**
 * @brief 从源码中void ChartPrivate::saveXmlChart(QXmlStreamWriter &writer) const函数看
 *        部分ChartType类型还不支持，如CT_StockChart
 */
void Test4::on_pushButton_clicked()
{
    Document xlsx;               // 创建一个Excel对象（默认有一个Sheet1）
    for(int i = 1; i < 10; i++)
    {
        xlsx.write(i, 1, i * i * i);    // 在第一列写入9个数据
        xlsx.write(i, 2, i * i);       // 在第二列写入9个数据
        xlsx.write(i, 3, i *i - 1);    // 写入第三列数据
    }
    qDebug() << CellReference(1, 2).toString();                    // 可将行列号转换为【字符串】单元格引用

    // 插入面积图
    xlsx.write(3, 4, "CT_AreaChart");  // 在图标左上角单元格中写入图标的类型
    Chart* areaChart = xlsx.insertChart(3, 3, QSize(300, 300));
    areaChart->setChartType(Chart::CT_AreaChart);
    areaChart->addSeries(CellRange("A1:C9"));

    // 插入3D面积图（在WPS中显示存在问题，office没有测试）
    xlsx.write(3, 10, "CT_Area3DChart");
    Chart *area3DChart = xlsx.insertChart(3, 9, QSize(300, 300));
    area3DChart->setChartType(Chart::CT_Area3DChart);
    area3DChart->addSeries(CellRange("A1:C9"));

    // 插入折线图
    xlsx.write(3, 16, "CT_LineChart");
    Chart* lineChart = xlsx.insertChart(3, 15, QSize(300, 300));
    lineChart->setChartType(Chart::CT_LineChart);
    lineChart->addSeries(CellRange("A1:C9"));

    // 插入3D折线图
    xlsx.write(23, 4, "CT_Line3DChart");
    Chart* line3DChart = xlsx.insertChart(23, 3, QSize(300, 300));
    line3DChart->setChartType(Chart::CT_Line3DChart);
    line3DChart->addSeries(CellRange("A1:C9"));

    // 插入股价图（貌似还不支持）
    xlsx.write(23, 10, "CT_StockChart");
    Chart* stockChart = xlsx.insertChart(23, 9, QSize(300, 300));
    stockChart->setChartType(Chart::CT_StockChart);
    stockChart->addSeries(CellRange("A1:C9"));

    // 插入雷达图（貌似还不支持）
    xlsx.write(23, 16, "CT_RadarChart");
    Chart* radarChart = xlsx.insertChart(23, 15, QSize(300, 300));
    radarChart->setChartType(Chart::CT_RadarChart);
    radarChart->addSeries(CellRange("A1:A9"));

    // 插入散点图（在WPS中效果和CT_LineChart一样）
    xlsx.write(43, 4, "CT_ScatterChart");
    Chart* scatterChart = xlsx.insertChart(43, 3, QSize(300, 300));
    scatterChart->setChartType(Chart::CT_ScatterChart);
    scatterChart->addSeries(CellRange("A1:A9"));             // 插入三个数据系列
    scatterChart->addSeries(CellRange("B1:B9"));
    scatterChart->addSeries(CellRange("C1:C9"));
    // 散点图不能以A1:C9这种方式同时选择三列数据，在WPS中会默认把第一列数据当做X轴数据，QXlsx中会直接舍弃掉第一列数据，
    // 由addSeries函数中if (d->chartType == CT_ScatterChart || d->chartType == CT_BubbleChart)可看出
//    scatterChart->addSeries(CellRange("A1:C9"));

    // 插入饼图
    xlsx.write(43, 10, "CT_PieChart");
    Chart* pieChart = xlsx.insertChart(43, 9, QSize(300, 300));    // 在第三行、三列的单元格右下角位置插入一个长宽为300的图表
    pieChart->setChartType(Chart::CT_PieChart);                    // 指定图表类型为【饼图】（支持16种类型图表）
    pieChart->addSeries(CellRange("A1:A9"));                       // 添加饼图的数据系列1（单元格引用字符串方式指定【第一列数据】）
    pieChart->addSeries(CellRange(CellReference(1, 2), CellReference(9, 2))); // 添加饼图数据2（通过CellReference指定【第二列数据】）
    pieChart->addSeries(CellRange(1, 3, 9, 3));                    // 添加饼图数据系列3（通过[开始行列号]和[结束行列号]指定【第三列数据】）

    // 插入3D饼图（这个图表在WPS中样式和CT_PieChart一样，没有表现出3D效果，无法设置三维旋转）
    xlsx.write(43, 16, "CT_Pie3DChart");
    Chart* pie3DChart = xlsx.insertChart(43, 15, QSize(300, 300));
    pie3DChart->setChartType(Chart::CT_Pie3DChart);
    pie3DChart->addSeries(CellRange("A1:A9"));

    // 插入圆环图
    xlsx.write(63, 4, "CT_DoughnutChart");
    Chart* doughnutChart = xlsx.insertChart(63, 3, QSize(300, 300));
    doughnutChart->setChartType(Chart::CT_DoughnutChart);
    doughnutChart->addSeries(CellRange("A1:A9"));

    // 插入柱状图
    xlsx.write(63, 10, "CT_BarChart");
    Chart* barChart = xlsx.insertChart(63, 9, QSize(300, 300));
    barChart->setChartType(Chart::CT_BarChart);
    barChart->addSeries(CellRange("A1:A9"));

    // 插入3D柱状图（在WPS中显示异常，不支持3D柱状图）
    xlsx.write(63, 16, "CT_Bar3DChart");
    Chart* bar3DChart = xlsx.insertChart(63, 15, QSize(300, 300));
    bar3DChart->setChartType(Chart::CT_Bar3DChart);
    bar3DChart->addSeries(CellRange("A1:A9"));

    // 插入饼图图（还不支持）
    xlsx.write(83, 4, "CT_OfPieChart");
    Chart* ofPieChart = xlsx.insertChart(83, 3, QSize(300, 300));
    ofPieChart->setChartType(Chart::CT_OfPieChart);
    ofPieChart->addSeries(CellRange("A1:A9"));

    // 插入曲面图（还不支持）
    xlsx.write(83, 10, "CT_SurfaceChart");
    Chart* surfaceChart = xlsx.insertChart(83, 9, QSize(300, 300));
    surfaceChart->setChartType(Chart::CT_SurfaceChart);
    surfaceChart->addSeries(CellRange("A1:A9"));

    // 插入3D曲面图（还不支持）
    xlsx.write(83, 16, "CT_Surface3DChart");
    Chart* surface3DChart = xlsx.insertChart(83, 15, QSize(300, 300));
    surface3DChart->setChartType(Chart::CT_Surface3DChart);
    surface3DChart->addSeries(CellRange("A1:A9"));

    // 插入气泡图（还不支持）
    xlsx.write(103, 4, "CT_BubbleChart");
    Chart* bubbleChart = xlsx.insertChart(103, 3, QSize(300, 300));
    bubbleChart->setChartType(Chart::CT_BubbleChart);
    bubbleChart->addSeries(CellRange("A1:A9"));

    xlsx.saveAs(EXCEL_NAME);
}
