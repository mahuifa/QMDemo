#ifndef DOWNLOADTHREADS_H
#define DOWNLOADTHREADS_H

#include "mapStruct.h"
#include <QFutureWatcher>
#include <QObject>

class DownloadThreads : public QObject
{
    Q_OBJECT
public:
    explicit DownloadThreads(QObject* parent = nullptr);
    ~DownloadThreads();

    // 传入需要下载的瓦片信息
    void getImage(QList<ImageInfo> infos);
    void quit();   // 退出下载

signals:
    void finished(ImageInfo info);   // 返回下载后的瓦片，由于QImage为共享内存，所以传递不需要考虑太多性能

private:
    QFuture<void> m_future;
    QList<ImageInfo> m_infos;
};

#endif   // DOWNLOADTHREADS_H
