#include "test7.h"
#include "ui_test7.h"
#include <QDebug>
#include <xlsxdocument.h>
QXLSX_USE_NAMESPACE

#define EXCEL_NAME "test7.xlsx"         // 本Demo使用的Excel文件名

Test7::Test7(QWidget *parent) :
    InterFace(parent),
    ui(new Ui::Test7)
{
    ui->setupUi(this);
    this->setWindowTitle("QXlsx设置字体格式、样式");
    this->setToolTip(this->windowTitle());
}

Test7::~Test7()
{
    delete ui;
}

QString Test7::getExcelName()
{
    return EXCEL_NAME;
}

/**
 * @brief 通过索引号设置数字格式
 */
void Test7::on_pushButton_clicked()
{
    Document xlsx;

    Format format;
    for(int i = 1; i < 100; i++)
    {
        format.setNumberFormatIndex(i);
        xlsx.write(i, 1, 1234, format);
        xlsx.write(i, 3, 1234.321, format);
        qDebug() << xlsx.cellAt(i, 1)->format().numberFormatIndex();  // 获取当前单元格的数字格式
    }

    if(xlsx.saveAs(EXCEL_NAME))
    {
        qInfo() << "Excel保存成功！";
    }
    else
    {
        qWarning() << "Excel保存失败！";
    }
}

/**
 * @brief 设置自定义数字格式
 */
void Test7::on_pushButton_2_clicked()
{
    Document xlsx;

    Format format;
    format.setNumberFormat("# ?/?");
    xlsx.write(1, 1, 1234.321, format);
    format.setNumberFormat("[Red][<=100];[Green][>100]");
    xlsx.write(2, 1, 14.321, format);

    if(xlsx.saveAs(EXCEL_NAME))
    {
        qInfo() << "Excel保存成功！";
    }
    else
    {
        qWarning() << "Excel保存失败！";
    }
}

/**
 * @brief 设置字体样式
 */
void Test7::on_pushButton_3_clicked()
{
    Document xlsx;

    xlsx.write(1, 1, "默认样式");

    // 设置字体大小
    Format format;
    format.setFontSize(15);
    xlsx.write(1, 2, "字体大小15", format);
    qDebug() << xlsx.cellAt(1, 2)->format().fontSize();  // 获取当前单元格的字体大小

    // 设置字体斜体
    Format format1;
    format1.setFontItalic(true);
    xlsx.write(1, 3, "斜体", format1);
    qDebug() << xlsx.cellAt(1, 3)->format().fontItalic();  // 获取当前单元格的字体是否为 斜体

    // 设置字体删除线
    Format format2;
    format2.setFontStrikeOut(true);
    xlsx.write(1, 4, "删除线", format2);
    qDebug() << xlsx.cellAt(1, 4)->format().fontStrikeOut();  // 获取当前单元格的字体是否有 删除线

    // 设置字体颜色
    Format format3;
    format3.setFontColor(Qt::red);
    xlsx.write(1, 5, "字体颜色", format3);
    qDebug() << xlsx.cellAt(1, 5)->format().fontColor();  // 获取当前单元格的字体颜色

    // 设置字体加粗
    Format format4;
    format4.setFontBold(true);
    xlsx.write(1, 6, "字体加粗", format4);
    qDebug() << xlsx.cellAt(1, 6)->format().fontBold();  // 获取当前单元格的字体是否加粗

    // 设置字体特殊格式（上、下标）
    Format format5;
    format5.setFontScript(Format::FontScriptSub);   // 设置下标
    xlsx.write(1, 7, "字体下标", format5);
    format5.setFontScript(Format::FontScriptSuper); // 设置上标
    xlsx.write(1, 8, "字体上标", format5);
    qDebug() << xlsx.cellAt(1, 7)->format().fontScript();  // 获取当前单元格的字体的特殊格式

    // 设置下划线
    Format format6;
    format6.setFontUnderline(Format::FontUnderlineNone);
    xlsx.write(1, 9, "无下划线", format6);
    format6.setFontUnderline(Format::FontUnderlineSingle);
    xlsx.write(1, 10, "单下划线", format6);
    format6.setFontUnderline(Format::FontUnderlineDouble);
    xlsx.write(1, 11, "双下划线", format6);
    format6.setFontUnderline(Format::FontUnderlineSingleAccounting);
    xlsx.write(1, 12, "会计用单下划线", format6);
    format6.setFontUnderline(Format::FontUnderlineDoubleAccounting);
    xlsx.write(1, 13, "会计用双下划线", format6);
    qDebug() << xlsx.cellAt(1, 9)->format().fontUnderline();  // 获取当前单元格文本下划线格式

    // 设置字体轮廓
    Format format7;
    format7.setFontOutline(true);
    xlsx.write(1, 14, "字体轮廓", format7);
    qDebug() << xlsx.cellAt(1, 14)->format().fontOutline();  // 获取当前单元格是否打开字体轮廓

    // 设置字体类型
    Format format8;
    format8.setFontName("黑体");
    xlsx.write(1, 15, "字体类型", format8);
    qDebug() << xlsx.cellAt(1, 15)->format().fontName();  // 获取当前单元格字体类型

    if(xlsx.saveAs(EXCEL_NAME))
    {
        qInfo() << "Excel保存成功！";
    }
    else
    {
        qWarning() << "Excel保存失败！";
    }
}
