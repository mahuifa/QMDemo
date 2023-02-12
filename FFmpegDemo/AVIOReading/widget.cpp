#include "widget.h"
#include "ui_widget.h"
#include <qfiledialog.h>
#include <QDebug>
#include <qthread.h>
#include <qtimer.h>

extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavutil/file.h"
}

typedef struct BufferData {
    uchar*  ptr;
    size_t  size;   // 缓冲区中剩余的大小
}BufferData;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("AVIOContext访问的自定义缓冲区读取数据 V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 选择文件
 */
void Widget::on_pushButton_clicked()
{
    QString strName = QFileDialog::getOpenFileName(this, "选择播放视频~！", "/", "视频 (*.mp4 *.m4v *.mov *.avi *.flv);; 其它(*)");
    if(strName.isEmpty())
    {
        return;
    }
    ui->line_file->setText(strName);
}

void Widget::on_pushButton_2_clicked()
{
    int ret = openAV();
    if(ret < 0)
    {
        showError(ret);
    }
    av_file_unmap(m_buffer, m_bufSize);           // 释放m_buffer
    avformat_free_context(m_formatContext);       // 释放m_formatContext
    if(m_avioContext)
    {
        av_freep(&m_avioContext->buffer);         // 释放m_avioBuffer
    }
    avio_context_free(&m_avioContext);            // 释放m_avioContext并置NULL

    m_avioBuffer = nullptr;
    m_buffer = nullptr;
    m_bufSize = 0;
    m_formatContext = nullptr;
}

/**
 * @brief    自定义非阻塞延时
 * @param ms
 */
void msleep(int ms)
{
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, SLOT(quit()));
    loop.exec();

}

/**
 * @brief           回调读取数据
 * @param opaque
 * @param buf
 * @param buf_size
 * @return
 */
int Widget::read_packet(void *opaque, uint8_t *buf, int buf_size)
{
    BufferData *bd = static_cast<BufferData *>(opaque);    // bd指针指向了读取文件的所有数据
    buf_size = FFMIN(buf_size, int(bd->size));             // 获取最小值

    if (!buf_size)
    {
        return AVERROR_EOF;       // 文件结尾
    }
    qDebug() << QString("当前指向缓冲区位置ptr：0x%1     剩余数据长度size：%2").arg(quint64(bd->ptr), 0, 16).arg(bd->size);

    /* 将内部缓冲区数据复制到buf */
    memcpy(buf, bd->ptr, quint64(buf_size));
    bd->ptr  += buf_size;                     // 通过指针向后移动读取数据
    bd->size -= quint64(buf_size);            // 每读取一次则剩余长度减4096

    msleep(1);   // 加上延时，否则回调函数执行很快，不能用QThread延时

    return buf_size;
}

int Widget::openAV()
{
    QString strName = ui->line_file->text();
    if(strName.isEmpty())
    {
        return AVERROR(ENOENT);     // 返回文件不存在的错误码
    }

    // 打开strName文件，将文件中所有数据读取到m_buffer中，读取的数据长度为m_bufSize，最后两个参数与日志相关，基本用不到
    int ret = av_file_map(strName.toStdString().data(), &m_buffer, &m_bufSize, 0, nullptr);
    if(ret < 0)
    {
        return ret;
    }
    showLog(QString("文件总buf：0x%1     文件总长度：%2").arg(quint64(m_buffer), 0, 16).arg(m_bufSize));

    m_formatContext = avformat_alloc_context();      // 分配一个解封装上下文，包含了媒体流的格式信息（.mp4 .avi）
    if(!m_formatContext)
    {
        return AVERROR(ENOMEM);        // 返回无法分配内存的错误码
    }

    m_avioBuffer = static_cast<uchar*>(av_malloc(quint64(m_avioBufSize))) ;  // 分配一个空间
    if(!m_avioBuffer)
    {
        return AVERROR(ENOMEM);        // 返回无法分配内存的错误码
    }
    showLog(QString("avioBuffer：0x%1     avioBufSize长度：%2").arg(quint64(m_avioBuffer), 0, 16).arg(m_avioBufSize));

    BufferData bufData;
    bufData.ptr = m_buffer;
    bufData.size = m_bufSize;
    m_avioContext = avio_alloc_context(m_avioBuffer,
                                       m_avioBufSize,
                                       0,
                                       &bufData,
                                       &read_packet,
                                       nullptr,
                                       nullptr);
    if(!m_avioContext)
    {
        return AVERROR(ENOMEM);        // 返回无法分配内存的错误码
    }

    m_formatContext->pb = m_avioContext;
    showLog(QString("缓冲区的开始：0x%1    缓冲区大小：%3").arg(quint64(m_avioContext->buffer), 0, 16).arg(m_avioContext->buffer_size));

    ret = avformat_open_input(&m_formatContext, nullptr, nullptr, nullptr);
    if(ret < 0)
    {
        return ret;
    }
    showLog("回调函数执行完成！");

    // 读取媒体文件的数据包以获取流信息。
    ret = avformat_find_stream_info(m_formatContext, nullptr);
    if(ret < 0)
    {
        return ret;
    }

    // 打印关于输入或输出格式的详细信息
    av_dump_format(m_formatContext,
                   0,                             // 要转储信息的流的索引
                   strName.toStdString().data(),  // 要打印的URL，例如源文件或目标文件
                   0);                            // 选择指定的上下文是输入(0)还是输出(1)

    return 0;
}

void Widget::showLog(const QString &log)
{
    ui->textEdit->append(log);
}

/**
 * @brief        显示ffmpeg函数调用异常信息
 * @param err
 */
void Widget::showError(int err)
{
    static char m_error[1024];
    memset(m_error, 0, sizeof (m_error));        // 将数组置零
    av_strerror(err, m_error, sizeof (m_error));
    showLog(QString("Error：%1  %2").arg(err).arg(m_error));
}

