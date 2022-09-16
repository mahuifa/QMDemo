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


void Widget::on_but_file_clicked()
{

}

void Widget::on_but_open_clicked()
{

}

void Widget::on_but_pause_clicked()
{

}

void Widget::on_playState(ReadThread::PlayState state)
{

}
