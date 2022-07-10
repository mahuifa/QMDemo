/******************************************************************************
 * @文件名     test6.h
 * @功能       1、通过【单元格引用】直接设置单元格合并；
 *            2、通过【行列号】设置单元格合并；
 *            3、在设置单元格合并时可以设置单元格【格式】，如文本居中对齐；
 *            4、取消单元格合并；
 *            5、在不修改其它内容情况下设置单元格水平对齐；
 *            6、在不修改其它内容情况下设置单元格垂直对齐。
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/07/09
 * @备注
 *****************************************************************************/
#ifndef TEST6_H
#define TEST6_H

#include <QWidget>
#include "Interface.h"

namespace Ui {
class Test6;
}

class Test6 : public InterFace
{
    Q_OBJECT

public:
    explicit Test6(QWidget *parent = nullptr);
    ~Test6();

    QString getExcelName() override;

private slots:
    void on_but_merge_clicked();

    void on_but_unmerge_clicked();

    void on_com_alignmentH_activated(int index);

    void on_com_alignmentV_activated(int index);

private:
    Ui::Test6 *ui;
};

#endif // TEST6_H
