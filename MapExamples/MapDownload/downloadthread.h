#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QObject>
#include <QNetworkAccessManager>

struct MapInfo
{

};

class DownloadThread : public QObject
{
    Q_OBJECT
public:
    explicit DownloadThread(QObject *parent = nullptr);
    ~DownloadThread();

signals:

private:
    void download();

private:
    QThread* m_thread = nullptr;
    QNetworkAccessManager m_manager;
};

#endif // DOWNLOADTHREAD_H
