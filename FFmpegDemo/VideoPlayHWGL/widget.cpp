#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>

extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
}
Q_DECLARE_METATYPE(AVFrame)  //注册结构体，否则无法通过信号传递AVFrame

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Qt+ffmpeg视频播放（软/硬解码 + OpenGL显示YUV/NV12）Demo V%1").arg(APP_VERSION));


    // 使用QOpenGLWindow绘制
    playImage = new PlayImage;
#if USE_WINDOW
    ui->verticalLayout->addWidget(QWidget::createWindowContainer(playImage));   // 这一步加载速度要比OpenGLWidget慢一点
#else
    ui->verticalLayout->addWidget(playImage);
#endif

    m_readThread = new ReadThread();
    connect(m_readThread, &ReadThread::repaint, playImage, &PlayImage::repaint, Qt::BlockingQueuedConnection);
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
        this->setWindowTitle(QString("Qt+ffmpeg视频播放（软/硬解码 + OpenGL显示YUV/NV12）Demo V%1").arg(APP_VERSION));
    }
}

void Widget::on_check_HW_clicked(bool checked)
{
    m_readThread->setHWDecoder(checked);
}
