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
        reset();
    }

    return  QWidget::eventFilter(watched, event);
}

/**
 * @brief  重置参数
 */
void Widget::reset()
{
    ui->horizontalSlider_origin->setMaximum(ui->renderArea->width());
    ui->verticalSlider_origin->setMaximum(ui->renderArea->height());

    ui->horizontalSlider_origin->setValue(0);
    ui->verticalSlider_origin->setValue(ui->renderArea->height());

    ui->dial->setValue(270);
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
    reset();
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
    pastValue = m_transform.isIdentity() ? 0 : pastValue;  // 重置时对pastValue进行初始化
    m_transform.translate(-pastValue, 0);                  // 由于QTransform设置时累加的，所以这里需要通过pastValue 减去之前的值

    pastValue = value;
    m_transform.translate(value,  0);
    ui->renderArea->setTransform(m_transform);
}

/**
 * @brief        垂直偏移原点
 * @param value
 */
void Widget::on_verticalSlider_origin_valueChanged(int value)
{
    static int pastValue = 0;
    pastValue = m_transform.isIdentity() ? 0 : pastValue;          // 重置时对pastValue进行初始化
    m_transform.translate(0, -pastValue);                          // 由于QTransform设置时累加的，所以这里需要通过pastValue 减去之前的值

    pastValue = ui->verticalSlider_origin->maximum() - value;
    m_transform.translate(0, pastValue);
    ui->renderArea->setTransform(m_transform);
}

/**
 * @brief          设置旋转功能
 * @param value
 */
void Widget::on_dial_valueChanged(int value)
{
    static int pastValue = 0;
    pastValue = m_transform.isIdentity() ? 0 : pastValue;
    m_transform.rotate(-pastValue);             // 由于QTransform中rotate是累计的，所以在设置之前这里进行重置
    pastValue = ((value + 90) % 360);

    m_transform.rotate(pastValue);      // 由于QDial默认0为竖直向下方向，QPainter默认0度为向右，所以需要+90度
    ui->renderArea->setTransform(m_transform);
}
