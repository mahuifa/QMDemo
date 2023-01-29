#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct AVFormatContext;
struct AVIOContext;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


    static int read_packet(void *opaque, uint8_t *buf, int buf_size);

private:
    void showError(int err);
    int  openAV();
    void showLog(const QString& log);

private:
    Ui::Widget *ui;

    AVFormatContext* m_formatContext = nullptr;
    AVIOContext    * m_avioContext   = nullptr;
    uchar          * m_buffer        = nullptr;    // 保存打开的媒体文件的所有数据
    size_t          m_bufSize       = 0;          // 打开的文件的总大小
    uchar          * m_avioBuffer    = nullptr;    // 从m_buffer中一次读取的数据
    int              m_avioBufSize   = 4096;       // 从m_buffer中一次读取的数据长度
};
#endif // WIDGET_H
