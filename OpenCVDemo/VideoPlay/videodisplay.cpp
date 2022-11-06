#include "videodisplay.h"
#include "ui_videodisplay.h"
#include <QtConcurrent>
#include <QCameraInfo>
#include <QFileDialog>

VideoDisplay::VideoDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoDisplay)
{
    ui->setupUi(this);


    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();   // 获取可用摄像头列表
    for(auto camera : cameras)
    {
        ui->com_cameras->addItem(camera.description());
    }

    m_cap = new VideoCapture();
    connect(this, &VideoDisplay::updateImage, ui->playImage, &PlayImage::updateImage);
}

VideoDisplay::~VideoDisplay()
{
    // 关闭并释放摄像头
    if(m_cap)
    {
        if(m_cap->isOpened())
        {
            quitThread();
        }
        delete m_cap;
    }
    delete ui;
}

/**
 * @brief 打开关闭摄像头
 */
void VideoDisplay::on_but_open_clicked()
{
    if(!m_cap->isOpened())
    {
        if(ui->com_cameras->count() <= 0) return;

        bool ret = m_cap->open(ui->com_cameras->currentIndex());
        if(ret)
        {
            m_play = true;
            ui->but_open->setText("关闭摄像头");
            QtConcurrent::run(this, &VideoDisplay::readImage);   // 在线程中读取
        }
    }
    else
    {
        quitThread();
        m_cap->release();
        ui->but_open->setText("打开摄像头");
    }
}

/**
 * @brief 打开本地视频或者网络视流，如rtsp、rtmp
 */
void VideoDisplay::on_but_play_clicked()
{
    if(!m_cap->isOpened())
    {
        QString url = ui->line_url->text();
        if(url.isEmpty()) return;
        bool ret = m_cap->open(url.toStdString());
        if(ret)
        {
            m_play = true;
            ui->but_play->setText("停止播放");
            QtConcurrent::run(this, &VideoDisplay::readImage);   // 在线程中读取
        }
    }
    else
    {
        quitThread();
        m_cap->release();
        ui->but_play->setText("开始播放");
    }
}

void VideoDisplay::on_but_file_clicked()
{
    QString str = QFileDialog::getOpenFileName(this, "视频文件", "./", "视频文件(*)");
    if(str.isNull()) return;

    ui->line_url->setText(str);
}

/**
 * @brief 退出opencv读取线程
 *        在一个线程打开关闭m_cap，在另一个线程异步读取需要先退出线程再关闭m_cap，否则可能导致m_cap关闭后又读取导致报错
 */
void VideoDisplay::quitThread()
{
    m_play = false;
    while (!m_play) {
        QThread::msleep(1);           // 等待线程退出
    }
    m_play = false;
}

/**
 * @brief 在线程中使用opencv读取图像
 */
void VideoDisplay::readImage()
{
    //获取当前摄像头的视频长宽信息
    Size s = Size(int(m_cap->get(CAP_PROP_FRAME_WIDTH)), int(m_cap->get(CAP_PROP_FRAME_HEIGHT)));
    VideoWriter   writer;
    double fps = m_cap->get(CAP_PROP_FPS);   // 获取帧率，有时候获取不到帧率，如摄像头，就使用默认帧率25
    if(fps < 1)
    {
        fps = 25;
    }
    writer.open("./1.avi", VideoWriter::fourcc('M','J','P','G'), fps, s, true);   // 打开保存视频文件，帧率25

    while (m_play && m_cap->isOpened())
    {
        bool ret = m_cap->read(mat);
        if(ret)
        {
            if(writer.isOpened())
            {
                writer.write(mat);   // 保存视频
            }
            emit this->updateImage(MatToQImage(mat));   // 将mat转换为Qimage并发送给显示界面
        }
        else
        {
            QThread::msleep(1);           // 防止频繁读取或者读取不到
        }

    }
    writer.release();
    m_play = true;
}

/**
 * @brief       Mat转QImage
 * @param mat
 * @return
 */
QImage VideoDisplay::MatToQImage(const Mat &mat)
{
    switch (mat.type())
    {
    case CV_8UC1:
        {
            QImage img(mat.data, mat.cols, mat.rows, mat.cols, QImage::Format_Grayscale8);
            return img;
        }
    case CV_8UC3:
        {
            QImage img(mat.data, mat.cols, mat.rows, mat.cols * 3, QImage::Format_RGB888);
            return img.rgbSwapped();
        }
    case CV_8UC4:
        {
            QImage img(mat.data, mat.cols, mat.rows, mat.cols * 4, QImage::Format_ARGB32);
            return img;
        }
    default:
        {
            return QImage();
        }
    }
}

