/******************************************************************************
 * @文件名     test1.h
 * @功能       QXlsx基本功能使用，主要包括
 *            创建Excel、打开Excel、写入数据、读取数据、Excel另存为、关闭Excel
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/06/18
 * @备注
 *****************************************************************************/
#ifndef TEST1_H
#define TEST1_H

#include <QWidget>
#include "xlsxdocument.h"
#include "Interface.h"

namespace Ui {
class Test1;
}

class Test1 :public InterFace
{
    Q_OBJECT

public:
    explicit Test1(QWidget *parent = nullptr);
    ~Test1();

    QString getExcelName() override;

private slots:
    void on_but_open_clicked();

    void on_but_init_clicked();
    void on_but_write_clicked();

    void on_but_recv_clicked();

    void on_but_saveAs_clicked();

    void on_but_close_clicked();

private:
    Ui::Test1 *ui;

    QXlsx::Document* m_xlsx = nullptr;
};

#endif // TEST1_H
