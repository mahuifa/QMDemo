/******************************************************************************
 * @文件名     test2.h
 * @功能       QXlsx操作工作表Demo，主要功能有
 *            1、查询打开的Excel中所有可用的工作表（Sheet）名称；
 *            2、创建指定名称的工作表（Sheet），自动添加到最末尾，支持ST_WorkSheet、ST_ChartSheet两种类型；
 *            3、在指定位置插入一个新的指定名称的工作表（Sheet），支持ST_WorkSheet、ST_ChartSheet两种类型；
 *            4、将某个工作表（Sheet）设置为当前活动工作表，并写入数据（ST_WorkSheet类型）；
 *            5、将指定名称的工作表（Sheet）重命名为一个新的名称，不改变其它因素；
 *            6、将指定名称的工作表（Sheet）拷贝为一个新的指定名称的工作表，两个工作表内容相同；
 *            7、将指定名称的工作表（Sheet）移动到指定位置；
 *            8、删除指定名称的工作表（Sheet）。
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/06/18
 * @备注
 *****************************************************************************/
#ifndef TEST2_H
#define TEST2_H

#include <QWidget>
#include "xlsxdocument.h"
#include "Interface.h"

namespace Ui {
class Test2;
}

class Test2 : public InterFace
{
    Q_OBJECT

public:
    explicit Test2(QWidget *parent = nullptr);
    ~Test2();

    QString getExcelName() override;

private slots:
    void on_but_open_clicked();

    void on_but_querySheet_clicked();

    void on_but_addSheet_clicked();

    void on_but_insert_clicked();

    void on_but_select_clicked();

    void on_but_rename_clicked();

    void on_but_close_clicked();

    void on_but_copy_clicked();

    void on_but_move_clicked();

    void on_but_delete_clicked();

private:
    Ui::Test2 *ui;

    QXlsx::Document* m_xlsx = nullptr;
};

#endif // TEST2_H
