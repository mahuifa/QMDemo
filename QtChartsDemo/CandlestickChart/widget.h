#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <qcandlestickset.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE       // 为了便于

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initChart();         // 初始化绘制图表
    QList<QCandlestickSet *> getData();  // 获取需要绘制数据

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
