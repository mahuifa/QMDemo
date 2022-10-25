#include "widget.h"
#include "ui_widget.h"
#include <qfiledialog.h>
#include <QDebug>
#include <qthread.h>
#include <qtimer.h>

extern "C" {        // 用C规则编译指定的代码
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <libavcodec/avcodec.h>
}

#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("使用libavcodec API的音频解码示例（mp3转pcm） V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
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

/**
 * @brief 获取输入文件路径
 */
void Widget::on_but_in_clicked()
{
    QString strName = QFileDialog::getOpenFileName(this, "选择用于解码的.mp3音频文件~！", "/", "音频 (*.mp3);");
    if(strName.isEmpty())
    {
        return;
    }
    ui->line_fileIn->setText(strName);
}

/**
 * @brief 获取解码后的原始音频文件保存路径
 */
void Widget::on_but_out_clicked()
{
    QString strName = QFileDialog::getSaveFileName(this, "解码后数据保存到~！", "/", "原始音频 (*.pcm);");
    if(strName.isEmpty())
    {
        return;
    }
    ui->line_fileOut->setText(strName);
}

void Widget::on_but_start_clicked()
{
    int ret = initDecode();
    if(ret < 0)
    {
        showError(ret);
    }

    avcodec_free_context(&m_context);   // 释放编解码器上下文和与之相关的所有内容，并将NULL写入提供的指针。
    av_parser_close(m_parserContex);
    av_frame_free(&m_frame);
    av_packet_free(&m_packet);
}

QString get_format_from_sample_fmt(int fmt)
{
    typedef struct sample_fmt_entry {
        enum AVSampleFormat sample_fmt;
        QString fmt_be;          // 大端模式指令
        QString fmt_le;          // 小端模式指令
    }sample_fmt_entry;

    sample_fmt_entry sample_fmt_entryes[] = {
        { AV_SAMPLE_FMT_U8,  "u8",    "u8"    },
        { AV_SAMPLE_FMT_S16, "s16be", "s16le" },
        { AV_SAMPLE_FMT_S32, "s32be", "s32le" },
        { AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
        { AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
    };

    for(int i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entryes); i++)
    {
        sample_fmt_entry entry = sample_fmt_entryes[i];
        if(fmt == entry.sample_fmt)
        {
            return AV_NE(entry.fmt_be, entry.fmt_le);   // AV_NE：判断大小端
        }
    }

    return QString();
}
/**
 * @brief   开始解码
 * @return
 */
int Widget::initDecode()
{
    QString strIn  = ui->line_fileIn->text();
    QString strOut = ui->line_fileOut->text();
    if(strIn.isEmpty() || strOut.isEmpty())
    {
        return AVERROR(ENOENT);        // 返回文件不存在的错误码
    }

    m_packet = av_packet_alloc();      // 创建一个AVPacket
    if(!m_packet)
    {
        return AVERROR(ENOMEM);        // 返回无法分配内存的错误码
    }

    m_frame = av_frame_alloc();      // 创建一个AVFrame
    if(!m_frame)
    {
        return AVERROR(ENOMEM);        // 返回无法分配内存的错误码
    }

    // 通过ID查询MPEG音频解码器
    m_codec = avcodec_find_decoder(AV_CODEC_ID_MP2);
    if(!m_codec)
    {
        return AVERROR(ENXIO);        // 找不到解码器
    }

    m_parserContex = av_parser_init(m_codec->id);
    if(!m_parserContex)
    {
        return AVERROR(ENOMEM);        // 解析器初始化失败
    }

    m_context = avcodec_alloc_context3(m_codec);  // 分配AVCodecContext并将其字段设置为默认值
    if(!m_context)
    {
        return AVERROR(ENOMEM);        // 解码器上下文创建失败
    }

    // 使用给定的AVCodec初始化AVCodecContext。
    int ret = avcodec_open2(m_context, m_codec, nullptr);
    if(ret < 0)
    {
        return ret;
    }

    // 打开输入文件
    QFile fileIn(strIn);
    if(!fileIn.open(QIODevice::ReadOnly))
    {
        return AVERROR(ENOENT);
    }
    // 打开输出文件
    QFile fileOut(strOut);
    if(!fileOut.open(QIODevice::WriteOnly))
    {
        return AVERROR(ENOENT);
    }

    showLog("开始解码！");
    msleep(1);
    QByteArray buf = fileIn.readAll();        // 读取所有数据
    char inbuf[AUDIO_INBUF_SIZE];
    while(buf.count() > 0)
    {
        int len = (buf.count() <= AUDIO_INBUF_SIZE) ? buf.count() : AUDIO_INBUF_SIZE;
        memcpy(inbuf, buf.data(), len);
        // 解析数据包
        ret = av_parser_parse2(m_parserContex, m_context, &m_packet->data, &m_packet->size,
                               reinterpret_cast<const uchar*>(inbuf),        // 这里不能直接使用buf.data(),否则会出现[mp2 @ 000001c8dbd40b00] Multiple frames in a packet.
                               len,
                               AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if(ret < 0)
        {
            break;
        }
        buf.remove(0, ret);  // 移除已解析的数据

        if(m_packet->size)
        {
            ret = decode(fileOut);
            if(ret < 0)
            {
//                return ret;
            }
        }
    }
    m_packet->data = nullptr;
    m_packet->size = 0;
    decode(fileOut);               // 需要传入空的数据帧才可以将解码器中所有数据读取出来

    enum AVSampleFormat sfmt = m_context->sample_fmt;
    // 检查样本格式是否为平面
    if(av_sample_fmt_is_planar(sfmt))
    {
        const char* name = av_get_sample_fmt_name(sfmt);  // 获取音频样本格式名称
        showLog(QString("警告：解码器生成的样本格式是平面格式（%1）。此示例将仅输出第一个通道。").arg(name));
        sfmt = av_get_packed_sample_fmt(sfmt);   // 获取样本格式的替代格式
    }

    // 音频通道数
#if FF_API_OLD_CHANNEL_LAYOUT
    int channels = m_context->channels;
#else
    int channels = m_context->ch_layout.nb_channels;
#endif
    QString strFmt = get_format_from_sample_fmt(sfmt);
    if(!strFmt.isEmpty())
    {
        showLog(QString("使用下列命令播放输出音频文件！\n"
                        "ffplay -f %1 -ac %2 -ar %3 %4\n")
                        .arg(strFmt).arg(channels)
                        .arg(m_context->sample_rate).arg(strOut));
    }

    return 0;
}

/**
 * @brief           解码并写入文件
 * @param fileOut
 * @return
 */
int Widget::decode(QFile &fileOut)
{
    // 将包含压缩数据的数据包发送到解码器
    int ret = avcodec_send_packet(m_context, m_packet);   // 注意：官方Demo中这里如果返回值<0则终止程序，由于数据中有mp3文件头，所以一开始会有返回值<0的情况

    // 读取所有输出帧（通常可以有任意数量的输出帧
    while (ret >= 0)
    {
        // 读取解码后的数据帧
        int ret = avcodec_receive_frame(m_context, m_frame);
        if(ret == AVERROR(EAGAIN)   // 资源暂时不可用
        || ret == AVERROR_EOF)      // 文件末尾
        {
            return 0;
        }
        else if(ret < 0)
        {
            return ret;
        }

        // 返回每个样本的字节数。例如格式为AV_SAMPLE_FMT_U8，则字节数为1字节
        int size = av_get_bytes_per_sample(m_context->sample_fmt);   // 返回值不会小于0
        for(int i = 0; i < m_frame->nb_samples; ++i)   // 音频样本数（采样率）
        {
#if FF_API_OLD_CHANNEL_LAYOUT
            for(int j = 0; j < m_context->channels; ++j)         // 5.1.2以后版本会弃用channels
#else
            for(int j = 0; j < m_context->ch_layout.nb_channels; ++j)
#endif
            {
                fileOut.write((const char*)(m_frame->data[j] + size * i), size);
            }
        }
    }
    return 0;
}
