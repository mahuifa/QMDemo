#include "test4.h"
#include "ui_test4.h"

#include <qdir.h>
#include <qprocess.h>
#include <QDebug>
#include <xlsxdocument.h>
#include <xlsxchart.h>
#include <xlsxchartsheet.h>
QXLSX_USE_NAMESPACE

#define EXCEL_NAME "test4.xlsx"         // 本Demo使用的Excel文件名

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
 * @brief 演示在工作表中插入图表，这里演示了Qxlsx中所有图表类型；
 *        从源码中void ChartPrivate::saveXmlChart(QXmlStreamWriter &writer) const函数看
 *        部分ChartType类型还不支持，如CT_StockChart
 */
void Test4::on_pushButton_clicked()
{
    Document xlsx;                    // 创建一个Excel对象（默认有一个Sheet1）
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

    // 插入饼图（还不支持）
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

    if(xlsx.saveAs(EXCEL_NAME))    // 如果文件已经存在则覆盖
    {
        qInfo() << "插入所有图表Demo保存成功！";
    }
    else
    {
        qWarning() << "插入所有图表Demo保存失败！";
    }
}

/**
 * @brief 1、设置【图例】位置；
 *        2、设置图表【标题】；
 *        3、打开图表网格线；
 *        4、行列交换标头；
 *        5、设置插入的数据范围是否包含标题；
 *        6、插入图表，引用其它工作表数据。
 */
void Test4::on_pushButton_2_clicked()
{
    Document xlsx;

    for(int i = 1; i < 10; i++)
    {
        xlsx.write(1, i + 1, QString("Pos %1").arg(i));      // 写入列标题
        xlsx.write(2, i + 1, i * i * i);                     // 写入数据
        xlsx.write(3, i + 1, i * i);
    }
    // 写入行标题
    xlsx.write(2, 1, "Set 1");
    xlsx.write(3, 1, "Set 2");

    // 插入一个柱状图，并设置图例在【右边】
    xlsx.write(5, 4, "图例在右边");
    Chart* barChart1 = xlsx.insertChart(5, 3, QSize(300, 300));  // 插入图表
    barChart1->setChartType(Chart::CT_BarChart);
    barChart1->setChartLegend(Chart::Right);     // 设置图例在右边，可设置None：无图例, Left：左边, Right：右边， Top：上边, Bottom：下边
    barChart1->setChartTitle("Test1");
    barChart1->addSeries(CellRange(1, 1, 3, 10), nullptr, true, true, false);

    // 插入一个柱状图，启动【主网格线】
    xlsx.write(5, 10, "图例在左边，启动主网格线");
    Chart* barChart2 = xlsx.insertChart(5, 9, QSize(300, 300));  // 插入图表
    barChart2->setChartType(Chart::CT_BarChart);
    barChart2->setChartLegend(Chart::Left);
    barChart2->setChartTitle("Test2");
    barChart2->setGridlinesEnable(true);   // 启动主网格线
    barChart2->addSeries(CellRange(1, 1, 3, 10), nullptr, true, true, false);

    // 插入一个柱状图，启动【次网格线】
    xlsx.write(5, 16, "图例在上边，启动次网格线");
    Chart* barChart3 = xlsx.insertChart(5, 15, QSize(300, 300));  // 插入图表
    barChart3->setChartType(Chart::CT_BarChart);
    barChart3->setChartLegend(Chart::Top);
    barChart3->setChartTitle("Test3");
    barChart3->setGridlinesEnable(false, true);  // 关闭主网格线，启动子网格线
    barChart3->addSeries(CellRange(1, 1, 3, 10), nullptr, true, true, false);

    // 插入一个柱状图，【行列交换标头】
    xlsx.write(25, 4, "图例在下边，行列交换标头");
    Chart* barChart4 = xlsx.insertChart(25, 3, QSize(300, 300));  // 插入图表
    barChart4->setChartType(Chart::CT_BarChart);
    barChart4->setChartLegend(Chart::Bottom);
    barChart4->setChartTitle("Test4");
    barChart4->addSeries(CellRange(1, 1, 3, 10), nullptr, false, true, true);  // 参数5【true：以1列为1个数据系列，false：以1行为1个数据系列】

    // 插入一个柱状图，【数据范围不包含标题】
    xlsx.write(25, 10, "数据范围不包含标题");
    Chart* barChart5 = xlsx.insertChart(25, 9, QSize(300, 300));  // 插入图表
    barChart5->setChartType(Chart::CT_BarChart);
    barChart5->setChartLegend(Chart::Right);
    barChart5->setChartTitle("Test5");
    // 参数1：添加数据系列范围；参数2：指定插入的数据位于哪个工作表（Sheet），默认为NULL，即当前工作表；
    // 参数3，数据系列范围第一行是否为列标题，true：为标题；参数4，数据系列范围第1列是否为行标题，true：为标题；默认都不为标题
    // 参数5：交换行列标头。
    barChart5->addSeries(CellRange(1, 1, 3, 10));

    // 插入一个柱状图，【数据范围包含列标题】
    xlsx.write(25, 16, "数据范围包含列标题");
    Chart* barChart6 = xlsx.insertChart(25, 15, QSize(300, 300));  // 插入图表
    barChart6->setChartType(Chart::CT_BarChart);
    barChart6->setChartLegend(Chart::Right);
    barChart6->setChartTitle("Test6");
    barChart6->addSeries(CellRange(1, 1, 3, 10), nullptr, true);

    // 插入一个柱状图，【数据范围包含行标题】
    xlsx.write(45, 4, "数据范围包含行标题");
    Chart* barChart7 = xlsx.insertChart(45, 3, QSize(300, 300));  // 插入图表
    barChart7->setChartType(Chart::CT_BarChart);
    barChart7->setChartLegend(Chart::Right);
    barChart7->setChartTitle("Test7");
    barChart7->addSeries(CellRange(1, 1, 3, 10), nullptr, false, true);

    // 添加一个工作表（Sheet2）,在Sheet2中插入图表，数据为Sheet1中的数据
    xlsx.addSheet("Sheet2");                                     // 添加一个工作表，当前工作表为Sheet2
    xlsx.write(3, 4, "插入图表，引用Sheet1数据");
    Chart* barChart8 = xlsx.insertChart(3, 3, QSize(300, 300));  // 插入图表
    barChart8->setChartType(Chart::CT_BarChart);
    barChart8->setChartLegend(Chart::Right);
    barChart8->setChartTitle("Test8");
    barChart8->addSeries(CellRange(1, 1, 3, 10), xlsx.sheet("Sheet1"));   // 添加数据系列范围，并指定为Sheet1中的数据

    if(xlsx.saveAs(EXCEL_NAME))                             // 如果文件已经存在则覆盖
    {
        qInfo() << "保存成功！";
    }
    else
    {
        qWarning() << "保存失败！";
    }
}

/**
 * @brief 插入图表Sheet，并绘制一个柱状图
 */
void Test4::on_pushButton_3_clicked()
{
    Document xlsx;
    for(int i = 1; i < 10; i++)
    {
        xlsx.write(i, 1, i * i);         // 在Sheet1中写入1列数据
    }

    xlsx.addSheet("Chart1", AbstractSheet::ST_ChartSheet);               // 插入一个名称为【Chart1】，类型为【图表】的Sheet
    Chartsheet* sheet = static_cast<Chartsheet*>(xlsx.currentSheet());   // 获取当前工作表，并将类型转换为Chartsheet*
    Chart* barChart = sheet->chart();           // 图表Sheet中会默认内置一个Chart，从这一步开始就和正常操作图表一样了
    barChart->setChartType(Chart::CT_BarChart); // 设置图表类型位柱状图
    barChart->addSeries(CellRange("A1:A9"), xlsx.sheet("Sheet1")); // 添加数据系列，数据位于Sheet1中的A1-A9

    if(xlsx.saveAs(EXCEL_NAME))                             // 如果文件已经存在则覆盖
    {
        qInfo() << "保存成功！";
    }
    else
    {
        qWarning() << "保存失败！";
    }
}
