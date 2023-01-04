#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Qt+ffmpeg视频播放（软解码）-保存裸流Demo V%1").arg(APP_VERSION));

    m_readThread = new ReadThread();
    connect(m_readThread, &ReadThread::updateImage, ui->playImage, &PlayImage::updateImage, Qt::DirectConnection);
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

/**
 * @brief  获取本地视频路径
 */
void Widget::on_but_file_clicked()
{
    QString strName = QFileDialog::getOpenFileName(this, "选择播放视频~！", "/", "视频 (*.mp4 *.m4v *.mov *.avi *.flv);; 其它(*)");
    if(strName.isEmpty())
    {
        return;
    }
    ui->com_url->setCurrentText(strName);
}

/**
 * @brief  视频播放/停止
 */
void Widget::on_but_open_clicked()
{
    if(ui->but_open->text() == "开始播放")
    {
        m_readThread->open(ui->com_url->currentText());
    }
    else
    {
        m_readThread->close();
    }
}

/**
 * @brief 视频暂停/继续
 */
void Widget::on_but_pause_clicked()
{
    if(ui->but_pause->text() == "暂停")
    {
        m_readThread->pause(true);
        ui->but_pause->setText("继续");
    }
    else
    {
        m_readThread->pause(false);
        ui->but_pause->setText("暂停");
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
        ui->but_open->setText("停止播放");
    }
    else
    {
        ui->but_open->setText("开始播放");
        ui->but_pause->setText("暂停");
        this->setWindowTitle(QString("Qt+ffmpeg视频播放（软解码）-保存裸流Demo V%1").arg(APP_VERSION));
    }
}
