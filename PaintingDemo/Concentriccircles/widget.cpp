#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QPainter绘制时【数据类型】和【抗锯齿】对效果的影响 - V%1").arg(APP_VERSION));

    ui->widget_2->setAntialiased(true);   // 开启抗锯齿
    ui->widget_4->setAntialiased(true);
    ui->widget_3->setFloatBased(true);    // 使用浮点数据类型进行绘制
    ui->widget_4->setFloatBased(true);
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief         因为Qt的Demo使用定时器更新参数会一直刷新，不便于观察细节，这里改为通过滑动条设置绘制的参数
 * @param value
 */
void Widget::on_horizontalSlider_valueChanged(int value)
{
    ui->widget->setFrame(value);
    ui->widget_2->setFrame(value);
    ui->widget_3->setFrame(value);
    ui->widget_4->setFrame(value);
}
