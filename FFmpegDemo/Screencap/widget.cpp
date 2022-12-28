#include "widget.h"
#include "ui_widget.h"

#include <qfiledialog.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qt+ffmpeg录屏Demo V%1").arg(APP_VERSION));
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
        setSavePath();
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
        this->setWindowTitle(QString("正在录制：%1").arg(ui->line_path->text()));
        ui->but_open->setText("停止录制");
    }
    else
    {
        ui->but_open->setText("开始录屏");
        this->setWindowTitle(QString("Qt+ffmpeg录屏Demo V%1").arg(APP_VERSION));
    }
}

/**
 * @brief 设置文件保存路径
 */
void Widget::setSavePath()
{
    QString strDefault = QString("%1/Videos/%2").arg(QDir::homePath()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss"));
    QString strPath = QFileDialog::getSaveFileName(this, "视频保存到~",  strDefault,
                                                   "常用视频文件 (*.mp4 *.avi *.mov *.wmv *.flv *.h264 *.h265);;"
                                                   "其它文件格式 (*)");
    if(strPath.isEmpty()) return;

    ui->line_path->setText(strPath);
    m_readThread->setPath(strPath);
}
