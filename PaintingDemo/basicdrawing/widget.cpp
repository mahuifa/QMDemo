#include "widget.h"
#include "ui_widget.h"
#include "renderarea.h"
#include <QDebug>
#include <QMetaEnum>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    // 获取画图形状Shape枚举名称和值，写入下拉框
    QMetaEnum metaEnum = QMetaEnum::fromType<RenderArea::Shape>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_shape->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 画笔样式枚举
    metaEnum = QMetaEnum::fromType<Qt::PenStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_penStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 画笔笔帽样式枚举
    metaEnum = QMetaEnum::fromType<Qt::PenCapStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_penCapStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 线段转角处连接样式枚举
    metaEnum = QMetaEnum::fromType<Qt::PenJoinStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_penJoinStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 笔刷样式枚举
    metaEnum = QMetaEnum::fromType<Qt::BrushStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_brushStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }

    ui->widget->installEventFilter(this);
}

/**
 * @brief            事件过滤器
 * @param watched
 * @param event
 * @return
 */
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget && event->type() == QEvent::Resize)
    {
        ui->horizontalSlider_origin->setMaximum(ui->widget->width());
        ui->verticalSlider_origin->setMaximum(ui->widget->height());
        ui->horizontalSlider_origin->setValue(0);
        ui->verticalSlider_origin->setValue(ui->widget->height());
    }

    return  QWidget::eventFilter(watched, event);
}

/**
 * @brief         水平偏移原点
 * @param value
 */
void Widget::on_horizontalSlider_origin_valueChanged(int value)
{
    qDebug() << value;
}

/**
 * @brief        垂直偏移原点
 * @param value
 */
void Widget::on_verticalSlider_origin_valueChanged(int value)
{
    qDebug() << ui->widget->height() - value;
}

/**
 * @brief          旋转
 * @param value
 */
void Widget::on_horizontalSlider_rotate_valueChanged(int value)
{
    qDebug() << value;
}

void Widget::on_checkBox_clicked(bool checked)
{
    qDebug() << checked;
}
