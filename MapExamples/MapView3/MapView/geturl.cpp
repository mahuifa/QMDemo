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
    m_rect.setTopLeft(Bing::latLongToPixelXY(64.16, 56.115, m_level));
    m_rect.setBottomRight(Bing::latLongToPixelXY(148.66, 9.34, m_level));
}

GetUrl::~GetUrl()
{
    quit();
    clear();
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
    m_url = url;
    getImg(m_rect, m_level);
}

/**
 * @brief       下载瓦片
 * @param info
 * @return
 */
void httpGet(ImageInfo info)
{
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(info.url)));
    // 等待返回
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray buf = reply->readAll();
        info.img.loadFromData(buf);
    }
    else
    {
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

    emit GetUrlInterface::getInterface() -> update(info);
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

    quit();
    getTitle(rect, level);
    getUrl();
    m_future = QtConcurrent::map(m_infos, httpGet);
}

void GetUrl::getTitle(QRect rect, int level)
{
    QPoint tl = Bing::pixelXYToTileXY(rect.topLeft());
    QPoint br = Bing::pixelXYToTileXY(rect.bottomRight());

    QString value;
    ImageInfo info;
    info.z = level;
    for (int x = tl.x(); x < br.x(); x++)
    {
        info.x = x;
        for (int y = tl.y(); y < br.y(); y++)
        {
            value = QString("%1_%2_%3").arg(level).arg(x).arg(y);
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
    m_exist.clear();
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
