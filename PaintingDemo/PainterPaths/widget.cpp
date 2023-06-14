#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QPainterPath使用示例 - V%1").arg(APP_VERSION));
    ui->widget_2->setFill(true);
}

Widget::~Widget()
{
    delete ui;
}

