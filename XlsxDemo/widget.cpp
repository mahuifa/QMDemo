#include "widget.h"
#include "ui_widget.h"
#include "test1.h"

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


void Widget::on_but_demo1_clicked()
{
    if(m_widget)
    {
        delete m_widget;
        m_widget = nullptr;
    }
    m_widget = new Test1();
    m_widget->show();
}

