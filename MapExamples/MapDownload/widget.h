#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "downloadthread.h"
#include "downloadinfo.h"
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_but_savePath_clicked();

    void on_but_thread_clicked(bool checked);

    void on_but_threads_clicked(bool checked);

private:
    void finished(ImageInfo info);             // 通知下载完成的索引

private:
    Ui::Widget *ui;
    DownloadThread* m_dThread = nullptr;     // 单线程下载
    QList<ImageInfo> m_infos;                // 保存下载瓦片图片的信息
    QElapsedTimer m_timer;
};
#endif // WIDGET_H
