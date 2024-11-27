#include "widget.h"
#include "ui_widget.h"
#include <QCameraInfo>
#include <QFileDialog>
#include <QOpenGLContext>

extern "C"   // 用C规则编译指定的代码
{
#include "libavcodec/avcodec.h"
}
Q_DECLARE_METATYPE(AVFrame)   //注册结构体，否则无法通过信号传递AVFrame

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Qt+ffmpeg调用摄像头不解码直接显示YUYV图像Demo V%1").arg(APP_VERSION));

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

    // 获取可用摄像头列表
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for (auto camera : cameras)
    {
#if defined(Q_OS_WIN)
        ui->com_url->addItem("video=" + camera.description());   // ffmpeg在Window下要使用video=description()
#elif defined(Q_OS_LINUX)
        ui->com_url->addItem(camera.deviceName());   // ffmpeg在linux下要使用deviceName()
#elif defined(Q_OS_MAC)
#endif
    }
}

Widget::~Widget()
{
    // 释放视频读取线程
    if (m_readThread)
    {
        // 由于使用了BlockingQueuedConnection，所以在退出时如果信号得不到处理就会卡死，所以需要取消绑定
        disconnect(m_readThread, &ReadThread::repaint, playImage, &PlayImage::repaint);
        m_readThread->close();
        m_readThread->wait();
        delete m_readThread;
    }
    delete ui;
}

/**
 * @brief  视频播放/停止
 */
void Widget::on_but_open_clicked()
{
    if (ui->but_open->text() == "开始播放")
    {
        m_readThread->open(ui->com_url->currentText());
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
    if (state == ReadThread::play)
    {
        this->setWindowTitle(QString("正在播放：%1").arg(m_readThread->url()));
        ui->but_open->setText("停止播放");
    }
    else
    {
        ui->but_open->setText("开始播放");
        this->setWindowTitle(QString("Qt+ffmpeg调用摄像头不解码直接显示YUYV图像Demo V%1").arg(APP_VERSION));
    }
}
