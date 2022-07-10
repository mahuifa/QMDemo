#include "test6.h"
#include "ui_test6.h"
#include <QDebug>
#include <qmessagebox.h>
#include <xlsxcell_p.h>
#include <xlsxdocument.h>
QXLSX_USE_NAMESPACE

#define EXCEL_NAME "test6.xlsx"         // 本Demo使用的Excel文件名

Test6::Test6(QWidget *parent) :
    InterFace(parent),
    ui(new Ui::Test6)
{
    ui->setupUi(this);

    this->setWindowTitle("QXlsx合并单元格、文本对齐");
    this->setToolTip(this->windowTitle());
}

Test6::~Test6()
{
    delete ui;
}

QString Test6::getExcelName()
{
    return EXCEL_NAME;
}

/**
 * @brief 演示三种方式的单元格合并
 *        1、通过【单元格引用】直接设置单元格合并；
 *        2、通过【行列号】设置单元格合并；
 *        3、在设置单元格合并时可以设置单元格【格式】，如文本居中对齐
 */
void Test6::on_but_merge_clicked()
{
    Document xlsx;

    // 在Excel中写入三组数据
    xlsx.write("B3", "hello");
    xlsx.write("C3", "123");
    xlsx.write("B8", 123456);
    xlsx.write("E8", "北风卷地白草折，胡天八月即飞雪");

    xlsx.mergeCells("B3:F6");                  // 可以通过【单元格引用】直接设置单元格合并(注意，有数据的单元格应该时第一个位置)
    xlsx.mergeCells(CellRange(8, 2, 20, 3));   // 通过【行列号】设置单元格合并

    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);  // 水平居中
    format.setVerticalAlignment(Format::AlignVCenter);    // 垂直居中
    xlsx.mergeCells("E8:F20", format);                    // 在设置单元格合并时可以设置单元格【格式】，如文本居中对齐

    if(xlsx.saveAs(EXCEL_NAME))    // 如果文件已经存在则覆盖
    {
        qInfo() << "Excel保存成功！";
        // 保存成功后其它三种操作才可以用
        ui->but_unmerge->setEnabled(true);
        ui->com_alignmentH->setEnabled(true);
        ui->com_alignmentV->setEnabled(true);
    }
    else
    {
        qWarning() << "Excel保存失败！";
    }
}

/**
 * @brief 取消合并单元格
 */
void Test6::on_but_unmerge_clicked()
{
    Document xlsx(EXCEL_NAME);
    if(!xlsx.load())
    {
        QMessageBox::warning(this, "错误", QString("打开%1失败，可能是文件不存在！").arg(EXCEL_NAME));
        return;
    }

    xlsx.unmergeCells("B3:F6");   // 这里取消合并的范围【B3:F6】必须和之前合并单元格的【范围相同】，否则取消合并失败
    if(xlsx.save())
    {
        qInfo() << "Excel保存成功！";
    }
    else
    {
        qWarning() << "Excel保存失败！";
    }
}

/**
 * @brief    在不修改其它内容情况下设置单元格水平对齐
 * @param
 */
void Test6::on_com_alignmentH_activated(int index)
{
    Document xlsx(EXCEL_NAME);
    if(!xlsx.load())
    {
        QMessageBox::warning(this, "错误", QString("打开%1失败，可能是文件不存在！").arg(EXCEL_NAME));
        return;
    }

    Format format = xlsx.cellAt(8, 2)->format();                        // 获取单元格原有格式
    format.setHorizontalAlignment(Format::HorizontalAlignment(index));  // 设置单元格水平对齐格式
    xlsx.write(8, 2, xlsx.read(8, 2), format);                          // 将单元格原有内容、格式写入原来位置

    if(xlsx.save())
    {
        qInfo() << "Excel保存成功！";
    }
    else
    {
        qWarning() << "Excel保存失败！";
    }
}

/**
 * @brief       在不修改其它内容情况下设置单元格垂直对齐
 * @param index
 */
void Test6::on_com_alignmentV_activated(int index)
{
    Document xlsx(EXCEL_NAME);
    if(!xlsx.load())
    {
        QMessageBox::warning(this, "错误", QString("打开%1失败，可能是文件不存在！").arg(EXCEL_NAME));
        return;
    }

    Format format = xlsx.cellAt(8, 2)->format();                        // 获取单元格原有格式
    format.setVerticalAlignment(Format::VerticalAlignment(index));      // 设置单元格垂直对齐格式
    xlsx.write(8, 2, xlsx.read(8, 2), format);                          // 将单元格原有内容、格式写入原来位置

    if(xlsx.save())
    {
        qInfo() << "Excel保存成功！";
    }
    else
    {
        qWarning() << "Excel保存失败！";
    }
}
