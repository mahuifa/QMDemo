/******************************************************************************
 * @文件名     widget.h
 * @功能       该示例显示了如何创建简单的面积图，在源码中包含使用的说明注释和注意事项
 *             去除了Qt示例中的警告信息、一些过时的用法和危险的用法，
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/24
 * @备注
 *****************************************************************************/
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initChart();           // 初始化绘制图表

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
