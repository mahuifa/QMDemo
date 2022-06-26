/******************************************************************************
 * @文件名     test4.h
 * @功能       1、演示在工作表中插入所有QXlsx支持的图表类型和还不支持的类型，并通过WPS演示效果；
 *
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

private:
    Ui::Test4 *ui;
};

#endif // TEST4_H
