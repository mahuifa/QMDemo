#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QBoxPlotSeries>
QT_CHARTS_USE_NAMESPACE        // 引入命名空间，必须放在ui_widget.h前

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
    void initChart();                // 初始化绘制图表
    QList<QBoxSet *> getData();      // 获取用于绘制的盒须图数据
    qreal findMedian(QList<qreal>& sortedList, int begin, int end);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
