#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    // 使用QOpenGLWidget绘制
    m_openglWidget = new OpenGLWidget;
    ui->gridLayout->addWidget(m_openglWidget);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_but_widget_clicked()
{
    m_openglWidget->start();
}
