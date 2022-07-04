#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("Qt使用opencv打开摄像头、本地视频、网络视频流演示Demo");
}

Widget::~Widget()
{
    delete ui;
}

