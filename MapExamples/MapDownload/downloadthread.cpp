#include "downloadthread.h"

#include <QNetworkReply>
#include <QThread>

DownloadThread::DownloadThread(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_thread->start();
    connect(m_thread, &QThread::started, this, &DownloadThread::started);
    connect(m_thread, &QThread::finished, this, &DownloadThread::threadFinished);

    connect(this, &DownloadThread::getImage, this, &DownloadThread::startGet);
}

DownloadThread::~DownloadThread()
{
    m_thread->quit();
    m_thread->wait();
}

void DownloadThread::started()
{
    m_manager = new QNetworkAccessManager();
    connect(m_manager, &QNetworkAccessManager::finished, this, &DownloadThread::on_finished);
}

void DownloadThread::threadFinished()
{
    delete m_manager;
    m_manager = nullptr;
}

/**
 * @brief         开始发起下载瓦片请求
 * @param infos   传入包含需要下载信息的列表
 */
void DownloadThread::startGet(QList<ImageInfo>* infos)
{
    m_index = 0;
    m_infos = infos;
    get();
}

/**
 * @brief     发起get请求
 */
void DownloadThread::get()
{
    if(!m_infos || m_infos->isEmpty() || (m_index >= m_infos->count()))
    {
        return;
    }
    m_request.setUrl(m_infos->at(m_index).url);
    m_manager->get(m_request);
}

/**
 * @brief       接收到返回信息
 * @param reply
 */
void DownloadThread::on_finished(QNetworkReply *reply)
{
    ImageInfo& info = (*m_infos)[m_index];
    if(reply->error() == QNetworkReply::NoError)
    {
         info.img.loadFromData(reply->readAll());
         emit finished(m_index);
         m_index++;
         get();
    }
    else
    {
        if(info.count < 3)  // 下载失败三次则不再下载
        {
            info.count++;
            get();
            qDebug() << "下载失败：" << reply->errorString();
        }
        else
        {
            emit finished(m_index);
            m_index++;
        }
    }

    reply->deleteLater();
}
