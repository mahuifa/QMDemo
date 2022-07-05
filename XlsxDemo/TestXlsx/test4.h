/******************************************************************************
 * @文件名     test4.h
 * @功能       1、演示在工作表中插入所有QXlsx支持的图表类型和还不支持的类型，并通过WPS演示效果；
 *            2、设置【图例】位置；
 *            3、设置图表【标题】；
 *            4、打开图表网格线；
 *            5、行列交换标头；
 *            6、设置插入的数据范围是否包含标题；
 *            7、插入图表，引用其它工作表数据;
 *            8、插入图表Sheet，并绘制一个柱状图
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/06/26
 * @备注
 *****************************************************************************/
#ifndef TEST4_H
#define TEST4_H

#include <QWidget>
#include "Interface.h"

namespace Ui {
class Test4;
}

class Test4 :public InterFace
{
    Q_OBJECT

public:
    explicit Test4(QWidget *parent = nullptr);
    ~Test4();

    QString getExcelName() override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Test4 *ui;
};

#endif // TEST4_H
