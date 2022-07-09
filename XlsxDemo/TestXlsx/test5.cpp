#include "test5.h"
#include "ui_test5.h"
#include <QDebug>
#include <QMessageBox>
#include <xlsxdocument.h>
QXLSX_USE_NAMESPACE

#define EXCEL_NAME "test5.xlsx"         // 本Demo使用的Excel文件名

Test5::Test5(QWidget *parent) :
    InterFace(parent),
    ui(new Ui::Test5)
{
    ui->setupUi(this);

    this->setWindowTitle("QXlsx在工作表中插入、读取图片");
    this->setToolTip(this->windowTitle());
}

Test5::~Test5()
{
    delete ui;
}

QString Test5::getExcelName()
{
    return EXCEL_NAME;
}

/**
 * @brief 插入图片
 */
void Test5::on_but_insert_clicked()
{
    Document xlsx;

    QImage image1("://image/C++.PNG");
    QImage image2("://image/Qt.PNG");

    qDebug() << "插入图片："<<xlsx.insertImage(3, 3, image1);  // 在3行3列单元格右下角位置插入图片
    qDebug() << "插入图片："<<xlsx.insertImage(23, 3, image2);  // 在23行3列单元格右下角位置插入图片

    if(xlsx.saveAs(EXCEL_NAME))    // 如果文件已经存在则覆盖
    {
        qInfo() << "Excel保存成功！";
    }
    else
    {
        qWarning() << "Excel保存失败！";
    }
}

/**
 * @brief 打开Excel文件，并查询当前Sheet中图片数量
 */
void Test5::on_but_getCount_clicked()
{
    Document xlsx(EXCEL_NAME);
    if(!xlsx.load())
    {
        QMessageBox::warning(this, "错误", QString("打开%1失败，可能是文件不存在！").arg(EXCEL_NAME));
        return;
    }

    uint count = xlsx.getImageCount();       // 查询当前Sheet中图片数量
    QMessageBox::about(this, "插入图片数", QString("共有%1张图片！").arg(count));
}

/**
 * @brief 使用方法一读取Excel中的图片（通过索引读取）
 *        注意：这里索引从1开始，而不是从0开始（Qxlsx的一些小bug）
 */
void Test5::on_but_getImage1_clicked()
{
    Document xlsx(EXCEL_NAME);
    if(!xlsx.load())
    {
        QMessageBox::warning(this, "错误", QString("打开%1失败，可能是文件不存在！").arg(EXCEL_NAME));
        return;
    }

    QImage image;
    bool ret = xlsx.getImage(1, image);       // 读取当前Sheet中第1张图片(注意：索引是从1开始，而不是从0开始)
    if(ret)
    {
        ui->label->setPixmap(QPixmap::fromImage(image));   // 显示读取到的图片
    }
    else
    {
        QMessageBox::warning(this, "错误", "读取图片失败，可能是不存在！");
    }
}

/**
 * @brief 使用方法一读取Excel中的图片（通过行列号读取）
 */
void Test5::on_pushButton_4_clicked()
{
    Document xlsx(EXCEL_NAME);
    if(!xlsx.load())
    {
        QMessageBox::warning(this, "错误", QString("打开%1失败，可能是文件不存在！").arg(EXCEL_NAME));
        return;
    }

    QImage image;
    bool ret = xlsx.getImage(23, 3, image);       // 读取当前Sheet中第2张图片
    if(ret)
    {
        ui->label->setPixmap(QPixmap::fromImage(image));   // 显示读取到的图片
    }
    else
    {
        QMessageBox::warning(this, "错误", "读取图片失败，可能是不存在或位置错误！");
    }
}
