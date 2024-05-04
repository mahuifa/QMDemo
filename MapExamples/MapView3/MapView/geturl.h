#ifndef GETURL_H
#define GETURL_H

#include "mapStruct.h"
#include <qfuture.h>
#include <qset.h>
#include <QObject>

class GetUrlInterface : public QObject
{
    Q_OBJECT
public:
    static GetUrlInterface* getInterface()
    {
        static GetUrlInterface interface;
        return &interface;
    }

signals:
    void update(ImageInfo info);             // 传出下载的瓦片图信息
    void updateTitle(int x, int y, int z);   // 传出下载的瓦片编号

    void showRect(QRect rect);   // 设置显示像素范围
    void setLevel(int level);    // 设置瓦片层级
};

class GetUrl : public QObject
{
    Q_OBJECT
public:
    explicit GetUrl(QObject* parent = nullptr);
    ~GetUrl();

    void setUrl(QString url);   // 设置获取瓦片地图的源地址
    void getImg(QRect rect, int level);
    void showRect(QRect rect);
    void setLevel(int level);   // 设置瓦片层级

private:
    void getTitle(QRect rect, int level);    // 获取所有需要下载的瓦片地图编号
    void getUrl();                           // 获取用于请求瓦片地图的信息
    void clear();                            // 清空内容
    void quit();                             // 退出下载
    void updateTitle(int x, int y, int z);   // 传出下载的瓦片编号

private:
    QThread* m_thread = nullptr;
    QFuture<void> m_future;
    QRect m_rect;      // 显示瓦片地图像素范围
    int m_level = 5;   // 瓦片地图层级
    QString m_url;
    QSet<quint64> m_exist;        // 已经存在的瓦片地图编号
    QVector<ImageInfo> m_infos;   // 需要下载的瓦片地图信息
};

#endif   // GETURL_H
