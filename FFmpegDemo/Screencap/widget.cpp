#include "widget.h"
#include "ui_widget.h"
#include <QDateTime>
#include <qfiledialog.h>
#include <qstandardpaths.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qt+ffmpeg录屏Demo V%1").arg(APP_VERSION));
    m_readThread = new ReadThread();
    connect(m_readThread, &ReadThread::playState, this, &Widget::on_playState);
    connect(&m_timer, &QTimer::timeout, this, &Widget::on_timeout);
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
        if(!setSavePath()) return;
#if defined(Q_OS_WIN)
        m_readThread->open("desktop");
#elif defined(Q_OS_LINUX)
        m_readThread->open("");            // linux下设备名可以省略
#endif
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
        m_timer.start(1000);
        ui->timeEdit->setTime(QTime(0, 0, 0, 0));     // 这里重置不要使用clear()
    }
    else
    {
        ui->but_open->setText("开始录屏");
        this->setWindowTitle(QString("Qt+ffmpeg录屏Demo V%1").arg(APP_VERSION));
        m_timer.stop();
    }
}

/**
 * @brief 设置文件保存路径
 */
bool Widget::setSavePath()
{
    // 如果不指定文件后缀则在linux下默认保存的视频文件没有后缀，就无法通过后缀名推测视频保存格式
    QString strDefault = QString("%1/%2.mp4").arg(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation))
                                                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss"));
    QString strPath = QFileDialog::getSaveFileName(this, "视频保存到~",  strDefault,
                                                   "常用视频文件 (*.mp4 *.avi *.mov *.wmv *.flv *.h264 *.h265);;"
                                                   "其它文件格式 (*)");

    if(strPath.isEmpty()) return false;

    ui->line_path->setText(strPath);
    m_readThread->setPath(strPath);
    return true;
}

/**
 * @brief 更新计时器时间
 */
void Widget::on_timeout()
{
    ui->timeEdit->setTime(ui->timeEdit->time().addSecs(1));
}
