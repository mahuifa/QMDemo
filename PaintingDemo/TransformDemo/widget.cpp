#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_transform.translate(50, 50);
    m_transform.rotate(10);
    m_transform.scale(1, 1.0);

}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief          获取反转副本
 * @param checked
 */
void Widget::on_checkBox_transform_clicked(bool checked)
{
    if(checked)
    {
        ui->renderArea->setTransform(m_transform.inverted());
    }
    else
    {
        ui->renderArea->setTransform(m_transform);
    }
}
