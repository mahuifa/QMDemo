#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QObject>
#include "downloadinfo.h"
#include <QNetworkAccessManager>


class DownloadThread : public QObject
{
    Q_OBJECT
public:
    explicit DownloadThread(QObject *parent = nullptr);
    ~DownloadThread();

signals:
    void getImage(QList<ImageInfo>* infos);
    void finished(int index);             // 通知下载完成的索引

private:
    void started();         // 线程启动
    void threadFinished();  // 线程退出

    void startGet(QList<ImageInfo>* infos);
    void get();
    void on_finished(QNetworkReply *reply);

private:
    QThread* m_thread = nullptr;
    QNetworkAccessManager* m_manager = nullptr;
    QNetworkRequest m_request;
    QList<ImageInfo>* m_infos = nullptr;         // 保存需要下载的地图信息
    int m_index = 0;                             // 指向下载列表的索引
};

#endif // DOWNLOADTHREAD_H
