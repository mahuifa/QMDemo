/********************************************************************
 * 文件名： downloadthreads.cpp
 * 时间：   2024-03-31 20:32:58
 * 开发者：  mhf
 * 邮箱：   1603291350@qq.com
 * 说明：   多线程下载瓦片地图
 * ******************************************************************/
#include "downloadthreads.h"
#include <QtConcurrent>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>

static DownloadThreads* g_this = nullptr;
DownloadThreads::DownloadThreads(QObject *parent) : QObject(parent)
{
    g_this = this;  // 记录当前 this指针，用于传递信号
}

DownloadThreads::~DownloadThreads()
{
    g_this = nullptr;
    quit();
}

/**
 * @brief       下载瓦片
 * @param info
 * @return
 */
void getUrl(ImageInfo info)
{
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(info.url)));
    // 等待返回
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray buf = reply->readAll();
        info.img.loadFromData(buf);
    }
    else
    {
        info.count++;
        if(info.count < 3)
        {
            getUrl(info);   // 下载失败重新下载
            return;
        }
        else
        {
            qWarning() << "下载失败：" << reply->errorString();
        }
    }
    if(g_this)
    {
        emit g_this->finished(info);  // 通过信号将下载后的瓦片传出去
    }
}

/**
 * @brief         调用线程池下载瓦片
 * @param infos
 */
void DownloadThreads::getImage(QList<ImageInfo> infos)
{
    m_infos = infos;    // 这里不能使用infos，因为会在函数退出释放
#if 0   // 由于map使用的是全局线程池，所以可以查看、设置线程数
    qDebug() <<QThreadPool::globalInstance()->maxThreadCount();   // 查看最大线程数
    QThreadPool::globalInstance()->setMaxThreadCount(1);          // 设置最大线程数
#endif
    m_future = QtConcurrent::map(m_infos, getUrl);
}

/**
 * @brief 退出下载
 */
void DownloadThreads::quit()
{
    if(m_future.isRunning())   // 判断是否在运行
    {
        m_future.cancel();               // 取消下载
        m_future.waitForFinished();      // 等待退出
    }
}
