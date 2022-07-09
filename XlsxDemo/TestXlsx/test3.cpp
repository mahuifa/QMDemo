#include "test3.h"
#include "ui_test3.h"

#include <QDir>
#include <qprocess.h>
#include <QDebug>
#include "xlsxdocument.h"
QXLSX_USE_NAMESPACE            // 添加Xlsx命名空间

#define EXCEL_NAME "test3.xlsx"         // 本Demo使用的Excel文件名

Test3::Test3(QWidget *parent) :
    InterFace(parent),
    ui(new Ui::Test3)
{
    ui->setupUi(this);
    this->setWindowTitle("QXlsx查询设置工作表隐藏或可见状态Demo");
    this->setToolTip(this->windowTitle());
}

Test3::~Test3()
{
    delete ui;
}

QString Test3::getExcelName()
{
    return EXCEL_NAME;
}

/**
 * @brief 创建一个工作表
 */
void Test3::on_but_addSheet_clicked()
{
    Document xlsx(EXCEL_NAME);             // 如果文件存在则打开EXCEL_NAME，如果不存在则传入文件名，在save时保存为EXCEL_NAME
    qDebug() << xlsx.load();               // 如果文件存在，并且打开成功则为true，否则为false
//    xlsx.addSheet();                     // 创建一个工作表，如果没指定名称则默认为Sheet1、Sheet2递增
    xlsx.addSheet(ui->line_name->text());  // 指定名称创建工作表(默认设置为当前活动工作表)
    xlsx.write(2, 2, "hello");             // 在当前活动工作表写入数据
    if(xlsx.save())                        // 因为在【Document xlsx(EXCEL_NAME);】指定了文件名称，所以保存到EXCEL_NAME，如果没有指定则默认【Book1.xlsx】
    {
        qDebug() << "保存成功！";

        ui->com_sheets->clear();
        ui->com_sheets->addItems(xlsx.sheetNames());   // 查询所有可用工作表
    }
    else
    {
        qDebug() << "保存失败！";
    }
}

/**
 * @brief       通过下拉框选择工作表名称，查询工作表状态
 * @param arg1
 */
void Test3::on_com_sheets_activated(const QString &arg1)
{
    Document xlsx(EXCEL_NAME);
    AbstractSheet* sheet = xlsx.sheet(arg1);   // 根据名称获取工作表指针
    if(!sheet) return;    // 判断是否为NULL

    int state = sheet->sheetState();           // 获取当前工作表状态
    ui->com_state->setCurrentIndex(state);
}

/**
 * @brief         设置工作表状态，
 *                可通过setSheetState设置显示、隐藏、绝对隐藏【三种状态】，
 *                通过sheetState获取工作表当前状态【三种状态】
 *                也可以通过setHidden或setVisible设置显示、隐藏状态【两种状态】，
 *                并通过isHidden或者isVisible查询显示隐藏状态【两种状态】
 * @param index
 */
void Test3::on_com_state_activated(int index)
{
    Document xlsx(EXCEL_NAME);
    AbstractSheet* sheet = xlsx.sheet(ui->com_sheets->currentText());   // 根据名称获取工作表指针
    if(!sheet) return;    // 判断是否为NULL

    sheet->setSheetState(AbstractSheet::SheetState(index));             // 修改工作表状态
    if(xlsx.save())
    {
        qDebug() << "保存成功！" ;
    }
    else
    {
        qDebug() << "保存失败！";
    }
}
