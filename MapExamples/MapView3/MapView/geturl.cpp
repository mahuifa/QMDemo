/********************************************************************
 * 文件名： geturl.cpp
 * 时间：   2024-05-19 14:29:30
 * 开发者：  mhf
 * 邮箱：   1603291350@qq.com
 * 说明：   瓦片地图网络请求类
 * ******************************************************************/
#include "geturl.h"
#include "bingformula.h"
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <QDebug>
#include <QSet>
#include <QtConcurrent>

GetUrl::GetUrl(QObject* parent)
    : QObject{parent}
{
    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_thread->start();

    m_rect.setTopLeft(Bing::latLongToPixelXY(64.16, 56.115, m_level));
    m_rect.setBottomRight(Bing::latLongToPixelXY(148.66, 9.34, m_level));
    connect(GetUrlInterface::getInterface(), &GetUrlInterface::updateTitle, this, &GetUrl::updateTitle);
    connect(GetUrlInterface::getInterface(), &GetUrlInterface::showRect, this, &GetUrl::showRect);
    connect(GetUrlInterface::getInterface(), &GetUrlInterface::setLevel, this, &GetUrl::setLevel);
}

GetUrl::~GetUrl()
{
    quit();
    clear();

    m_thread->quit();
    m_thread->wait();
    delete m_thread;
}

/**
 * @brief     设置瓦片地图源地址
 * @param url
 */
void GetUrl::setUrl(QString url)
{
    if (url.isEmpty())
        return;

    quit();   // 退出下载后再清空数组，防止数据竞争
    clear();
    m_exist.clear();   // 清空已下载列表
    m_url = url;
    getImg(m_rect, m_level);   // 使用默认范围、层级更新地图
}

/**
 * @brief       下载瓦片
 * @param info
 * @return
 */
void httpGet(ImageInfo info)
{
    QNetworkAccessManager manager;
    QSharedPointer<QNetworkReply> reply(manager.get(QNetworkRequest(QUrl(info.url))));
    // 等待返回
    QEventLoop loop;
    QObject::connect(reply.data(), &QNetworkReply::finished, &loop, &QEventLoop::quit);   // 等待获取完成
    QTimer::singleShot(5000, &loop, &QEventLoop::quit);                                   // 等待超时
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray buf = reply->readAll();
        if (!buf.isEmpty())
        {
            info.img.loadFromData(buf);
            if (!info.img.isNull())
            {
                emit GetUrlInterface::getInterface() -> update(info);
                emit GetUrlInterface::getInterface() -> updateTitle(info.x, info.y, info.z);
                return;
            }
        }
    }

    info.count++;
    if (info.count < 3)
    {
        httpGet(info);   // 下载失败重新下载
        return;
    }
    else
    {
        qWarning() << "下载失败：" << reply->errorString();
    }
}

/**
 * @brief       获取瓦片地图
 * @param rect  瓦片地图的像素范围
 * @param level 瓦片地图的级别
 */
void GetUrl::getImg(QRect rect, int level)
{
    if (rect.isEmpty())
        return;
    if (level > 22 || level < 0)
        return;
    m_rect = rect;
    m_level = level;

    if (m_future.isRunning())   // 判断是否在运行
    {
        m_future.cancel();   // 取消下载
    }
    clear();   // 清空待下载列表

    getTitle(rect, level);   // 获取所有需要加载的瓦片编号
    qInfo() << "获取瓦片数：" << m_infos.count();
    getUrl();                                         // 将瓦片编号转为url
    m_future = QtConcurrent::map(m_infos, httpGet);   // 在线程池中下载瓦片图
}

/**
 * @brief      设置获取瓦片地图的像素范围
 * @param rect
 */
void GetUrl::showRect(QRect rect)
{
    if (rect.isEmpty())
        return;
    getImg(rect, m_level);
}

/**
 * @brief       通过设置显示瓦片层级别完成缩放显示
 * @param level
 */
void GetUrl::setLevel(int level)
{
    if ((level < 0) || (level > 23))
    {
        return;
    }
    if (m_level != level)
    {
        m_exist.clear();   // 清空已下载列表
    }
    m_level = level;
}

/**
 * @brief       获取瓦片编号
 * @param rect
 * @param level
 */
void GetUrl::getTitle(QRect rect, int level)
{
    QPoint tl = Bing::pixelXYToTileXY(rect.topLeft());
    QPoint br = Bing::pixelXYToTileXY(rect.bottomRight());

    quint64 value = 0;
    ImageInfo info;
    info.z = level;

    int max = qPow(2, level);   // 最大瓦片编号
    for (int x = tl.x(); x <= br.x(); x++)
    {
        if (x < 0)
            continue;
        if (x >= max)
            break;
        info.x = x;
        for (int y = tl.y(); y <= br.y(); y++)
        {
            if (y < 0)
                continue;
            if (y >= max)
                break;
            value = ((quint64) level << 48) + (x << 24) + y;

            if (!m_exist.contains(value))
            {
                info.y = y;
                m_infos.append(info);
            }
        }
    }
}

/**
 * @brief 获取用于请求瓦片地图的信息
 */
void GetUrl::getUrl()
{
    if (m_url.contains("{x}"))   // XYZ格式
    {
        QString url = m_url;
        url.replace("{x}", "%1");
        url.replace("{y}", "%2");
        url.replace("{z}", "%3");
        for (int i = 0; i < m_infos.count(); i++)
        {
            m_infos[i].url = url.arg(m_infos[i].x).arg(m_infos[i].y).arg(m_infos[i].z);
        }
    }
    else if (m_url.contains("{q}"))   // Bing的quadKey格式
    {
        QString url = m_url;
        url.replace("{q}", "%1");
        QPoint point;
        for (int i = 0; i < m_infos.count(); i++)
        {
            point.setX(m_infos[i].x);
            point.setY(m_infos[i].y);
            QString quadKey = Bing::tileXYToQuadKey(point, m_infos[i].z);   // 将xy转为quadkey
            m_infos[i].url = url.arg(quadKey);
        }
    }
    else
    {
        qDebug() << "url格式未定义";
    }
}

/**
 * @brief 清空内容
 */
void GetUrl::clear()
{
    QVector<ImageInfo> info;
    m_infos.swap(info);
}

/**
 * @brief 退出下载
 */
void GetUrl::quit()
{
    if (m_future.isRunning())   // 判断是否在运行
    {
        m_future.cancel();            // 取消下载
        m_future.waitForFinished();   // 等待退出
    }
}

/**
 * @brief     将下载成功的瓦片编号添加进已下载列表，已经下载的瓦片在后续不进行下载
 * @param x
 * @param y
 * @param z
 */
void GetUrl::updateTitle(int x, int y, int z)
{
    quint64 value = (quint64(z) << 48) + (x << 24) + y;
    m_exist.insert(value);
}
