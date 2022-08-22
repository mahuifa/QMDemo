#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

//    m_transform.translate(50, 50);
//    m_transform.rotate(10);
//    m_transform.scale(0.5, 1);

    ui->renderArea->installEventFilter(this);
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief            事件过滤器
 * @param watched
 * @param event
 * @return
 */
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->renderArea && event->type() == QEvent::Resize)
    {
        ui->horizontalSlider_origin->setMaximum(ui->renderArea->width());
        ui->verticalSlider_origin->setMaximum(ui->renderArea->height());

        ui->horizontalSlider_origin->setValue(0);
        ui->verticalSlider_origin->setValue(ui->renderArea->height());
    }

    return  QWidget::eventFilter(watched, event);
}

/**
 * @brief 设置原始变换
 */
void Widget::on_but_original_clicked()
{
    ui->renderArea->setTransform(m_transform);
}

/**
 * @brief  重置QTransform
 */
void Widget::on_but_reset_clicked()
{
    m_transform.reset();
    ui->renderArea->setTransform(m_transform);
}

/**
 * @brief 获取反转副本
 */
void Widget::on_but_transform_clicked()
{
    ui->renderArea->setTransform(m_transform.inverted());
}

/**
 * @brief        水平偏移原点
 * @param value
 */
void Widget::on_horizontalSlider_origin_valueChanged(int value)
{
    static int pastValue = 0;
    m_transform.translate(value - pastValue,  0);           // 由于QTransform设置时累加的，所以这里需要通过pastValue计算差值
    pastValue = value;
    ui->renderArea->setTransform(m_transform);
}

/**
 * @brief        垂直偏移原点
 * @param value
 */
void Widget::on_verticalSlider_origin_valueChanged(int value)
{
    static int pastValue = 0;
    m_transform.translate(0, ui->verticalSlider_origin->maximum() - value - pastValue);
    pastValue = ui->verticalSlider_origin->maximum() - value;
    ui->renderArea->setTransform(m_transform);
}
