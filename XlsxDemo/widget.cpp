#include "widget.h"
#include "ui_widget.h"
#include "test1.h"
#include "test2.h"
#include "test3.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("QXlsx读写Excel 示例程序");
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief          通过传入枚举值创建对象
 * @param type
 */
void Widget::createObject(Widget::DemoType type)
{
    if(m_widget)
    {
        delete m_widget;
        m_widget = nullptr;
    }
    switch (type)
    {
    case Demo1:
        m_widget = new Test1();
        break;
    case Demo2:
        m_widget = new Test2();
        break;
    case Demo3:
        m_widget = new Test3();
        break;
    }
    m_widget->show();
}


void Widget::on_but_demo1_clicked()
{
    createObject(Demo1);
}


void Widget::on_but_demo2_clicked()
{
    createObject(Demo2);
}


void Widget::on_but_demo3_clicked()
{
    createObject(Demo3);
}
