/******************************************************************************
 * @文件名     test5.h
 * @功能       1、在当前Sheet指定位置插入图片；
 *            2、获取当前Sheet中图片个数；
 *            3、通过索引方式和行列号方式读取当前Sheet中的图片。
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/07/06
 * @备注
 *****************************************************************************/
#ifndef TEST5_H
#define TEST5_H

#include <QWidget>
#include "Interface.h"

namespace Ui {
class Test5;
}

class Test5 : public InterFace
{
    Q_OBJECT

public:
    explicit Test5(QWidget *parent = nullptr);
    ~Test5();

    QString getExcelName() override;

private slots:
    void on_but_insert_clicked();

    void on_but_getCount_clicked();

    void on_but_getImage1_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Test5 *ui;
};

#endif // TEST5_H
