#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("Qt使用opencv打开摄像头、本地视频、网络视频流演示Demo - V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}

