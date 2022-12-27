#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_readThread = new ReadThread();
    connect(m_readThread, &ReadThread::playState, this, &Widget::on_playState);
}

Widget::~Widget()
{
    // 释放视频读取线程
    if(m_readThread)
    {
        m_readThread->close();
        m_readThread->wait();
        delete m_readThread;
    }
    delete ui;
}


void Widget::on_but_open_clicked()
{
    if(ui->but_open->text() == "开始录屏")
    {
        m_readThread->open("desktop");
//        m_readThread->open("C:/Users/mhf/Videos/1.mp4");
    }
    else
    {
        m_readThread->close();
    }
}

/**
 * @brief        根据视频播放状态切换界面设置
 * @param state
 */
void Widget::on_playState(ReadThread::PlayState state)
{
    if(state == ReadThread::play)
    {
        this->setWindowTitle(QString("正在播放：%1").arg(m_readThread->url()));
        ui->but_open->setText("停止录制");
    }
    else
    {
        ui->but_open->setText("开始录屏");
        this->setWindowTitle(QString("Qt+ffmpeg录屏Demo V%1").arg(APP_VERSION));
    }
}
