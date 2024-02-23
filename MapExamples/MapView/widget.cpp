#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QApplication>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(600, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QT下载瓦片地图简单示例--V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}

