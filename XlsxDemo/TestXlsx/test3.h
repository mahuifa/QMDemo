/******************************************************************************
 * @文件名     test3.h
 * @功能       基于AbstractSheet类的Demo，主要演示了
 *            1、创建工作表；
 *            2、查询工作表；
 *            3、查询工作表状态；
 *            4、设置工作表状态。
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/06/19
 * @备注
 *****************************************************************************/
#ifndef TEST3_H
#define TEST3_H

#include <QWidget>
#include "Interface.h"

namespace Ui {
class Test3;
}

class Test3 :  public InterFace
{
    Q_OBJECT

public:
    explicit Test3(QWidget *parent = nullptr);
    ~Test3();

    QString getExcelName() override;

private slots:

    void on_but_addSheet_clicked();

    void on_com_sheets_activated(const QString &arg1);

    void on_com_state_activated(int index);

private:
    Ui::Test3 *ui;
};

#endif // TEST3_H
