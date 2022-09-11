#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QPainterPath使用示例 - V%1").arg(APP_VERSION));
    ui->widget_2->setFill(true);
}

Widget::~Widget()
{
    delete ui;
}

