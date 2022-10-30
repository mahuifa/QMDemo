#ifndef WIDGET_H
#define WIDGET_H

#include <QFile>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct AVCodecParserContext;
struct AVCodecContext;
struct AVCodec;
struct AVPacket;
struct AVFrame;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_but_in_clicked();

    void on_but_out_clicked();

    void on_but_start_clicked();

private:
    int  initDecode();
    int  decode(QFile& fileOut);
    void showError(int err);
    void showLog(const QString& log);

private:
    Ui::Widget *ui;

    AVCodecParserContext*   m_parserContex  = nullptr;             // 裸流解析器
    AVCodecContext*         m_context       = nullptr;             // 解码器上下文
    const AVCodec*          m_codec         = nullptr;             // 音频解码器
    AVPacket*               m_packet        = nullptr;             // 未解码的原始数据
    AVFrame*                m_frame         = nullptr;             // 解码后的数据帧
};
#endif // WIDGET_H
