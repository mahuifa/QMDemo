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
}

Test4::~Test4()
{
    delete ui;
}

QString Test4::getExcelName()
{
    return EXCEL_NAME;
}


void Test4::on_pushButton_clicked()
{
    Document xlsx;               // 创建一个Excel对象（默认有一个Sheet1）
    for(int i = 1; i < 10; i++)
    {
        xlsx.write(i, 1, i * i *i);    // 在第一列写入9个数据
        xlsx.write(i, 2, i * i);       // 在第二列写入9个数据
        xlsx.write(i, 3, i *i - 1);    // 写入第三列数据
    }

    // 插入面积图
    Chart* areaChart = xlsx.insertChart(3, 3, QSize(300, 300));
    areaChart->setChartType(Chart::CT_AreaChart);
    areaChart->addSeries(CellRange("A1:C9"));

    // 插入3D面积图（3D面积图支持还不是很好，在WPS中显示存在问题，office没有测试）
    Chart *area3DChart = xlsx.insertChart(3, 9, QSize(300, 300));
    area3DChart->setChartType(Chart::CT_Area3DChart);
    area3DChart->addSeries(CellRange("A1:C9"));

    Chart* pieChart = xlsx.insertChart(23, 3, QSize(300, 300));    // 在第三行、三列的单元格右下角位置插入一个长宽为300的图表
    pieChart->setChartType(Chart::CT_PieChart);                   // 指定图表类型为【饼图】（支持16种类型图表）
    pieChart->addSeries(CellRange("A1:A9"));                      // 添加饼图的数据系列1（单元格引用字符串方式指定【第一列数据】）
    pieChart->addSeries(CellRange(CellReference(1, 2), CellReference(9, 2))); // 添加饼图数据2（通过CellReference指定【第二列数据】）
    qDebug() << CellReference(1, 2).toString();                   // 可将行列号转换为【字符串】单元格引用
    pieChart->addSeries(CellRange(1, 3, 9, 3));                   // 添加饼图数据系列3（通过[开始行列号]和[结束行列号]指定【第三列数据】）

    xlsx.saveAs(EXCEL_NAME);
}
