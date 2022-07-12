/******************************************************************************
 * @文件名     test7.h
 * @功能       1、通过索引号设置数字格式
 *            2、设置自定义数字格式
 *            3、设置字体样式
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/07/11
 * @备注
 *****************************************************************************/
#ifndef TEST7_H
#define TEST7_H

#include <QWidget>
#include "Interface.h"

namespace Ui {
class Test7;
}

class Test7 : public InterFace
{
    Q_OBJECT

public:
    explicit Test7(QWidget *parent = nullptr);
    ~Test7();

    QString getExcelName() override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Test7 *ui;
};

#endif // TEST7_H
