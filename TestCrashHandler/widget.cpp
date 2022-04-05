#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("Qt崩溃定位Demo");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    QWidget* w;
    delete  w;
}


void Widget::on_pushButton_2_clicked()
{
    int a= 0;
    int b = 100;
    int c = b / a;
}


void Widget::on_pushButton_3_clicked()
{
    QWidget* w;
    w->show();
}

