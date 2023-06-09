#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qt崩溃定位Demo - V%1").arg(APP_VERSION));
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
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

