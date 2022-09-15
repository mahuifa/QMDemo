#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Qt+ffmpeg视频播放Demo V%1").arg(APP_VERSION));

}

Widget::~Widget()
{
    delete ui;
}

