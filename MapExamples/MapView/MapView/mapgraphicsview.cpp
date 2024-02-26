#include "mapgraphicsview.h"

#include <QDir>
#include <QDebug>
#include <QGraphicsItemGroup>
#include <QtConcurrent>
#include <QWheelEvent>

MapGraphicsView* g_this = nullptr;
MapGraphicsView::MapGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    this->setScene(m_scene);
    g_this = this;
    connect(this, &MapGraphicsView::addImage, this, &MapGraphicsView::on_addImage);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
}

MapGraphicsView::~MapGraphicsView()
{
    g_this = nullptr;
    quit();   // 如果程序退出时还在调用map就会报错，所以需要关闭
}


/**
 * @brief 退出多线程
 */
void MapGraphicsView::quit()
{
    if(m_future.isRunning())   // 判断是否在运行
    {
        m_future.cancel();               // 取消多线程
        m_future.waitForFinished();      // 等待退出
    }
}


/**
 * @brief       设置加载显示的瓦片地图路径
 * @param path
 */
void MapGraphicsView::setPath(const QString &path)
{
    if(path.isEmpty()) return;
    m_path = path;
    getMapLevel();      // 获取瓦片层级
    loatImage();        // 加载第一层瓦片
}

/**
 * @brief        鼠标缩放地图
 * @param event
 */
void MapGraphicsView::wheelEvent(QWheelEvent *event)
{
    QGraphicsView::wheelEvent(event);

    if(event->angleDelta().y() > 0)   // 放大
    {
        if(m_keyIndex < m_mapItemGroup.count() -1)
        {
            m_keyIndex++;
        }
    }
    else
    {
        if(m_keyIndex > 0)
        {
            m_keyIndex--;
        }
    }
    loatImage();        // 加载新的层级瓦片
}

/**
 * @brief 计算瓦片层级
 */
void MapGraphicsView::getMapLevel()
{
    if(m_path.isEmpty()) return;

    clearHash();    // 加载新瓦片路径时将之前的内容清空

    QDir dir(m_path);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);    // 设置过滤类型为文件夹，且不包含隐藏文件夹
    dir.setSorting(QDir::Name);                          // 设置按文件夹名称排序
    QStringList dirs = dir.entryList();
    for(auto& strDir : dirs)
    {
        bool ok;
        int level = strDir.toInt(&ok);
        if(ok)
        {
            if(!m_mapItemGroup.contains(level))  // 如果不包含
            {
                m_mapItemGroup[level] = new QGraphicsItemGroup();
            }
        }
    }
}

/**
 * @brief 获取当前显示层级中所有瓦片的编号
 */
void MapGraphicsView::getTitle()
{
    QString path = m_path + QString("/%1").arg(getKey());    // z  第一层文件夹
    QDir dir(path);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);    // 设置过滤类型为文件夹，且不包含隐藏文件夹
    dir.setSorting(QDir::Name);                          // 设置按文件夹名称排序
    QStringList dirs = dir.entryList();
    QPoint point;
    for(auto& strDir : dirs)
    {
        bool ok;
        int x = strDir.toInt(&ok);                         // x层级 第二层文件夹
        if(ok)
        {
            point.setX(x);
            dir.setPath(path + QString("/%1").arg(x));
            dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);    // 设置过滤类型为文件，且不包含隐藏文件
            dir.setSorting(QDir::Name);                           // 设置按文件夹名称排序
            QStringList files = dir.entryList();
            for(auto& file: files)
            {
                int y = file.split('.').at(0).toInt(&ok);   // 去除后缀，以文件名为y
                if(ok)
                {
                    point.setY(y);
                    m_imgTitle.append(point);
                }
            }
        }
    }
}

QString g_path;   // 保存当前层级路径
/**
 * @brief       在多线程中加载图片
 * @param point
 */
void readImg(const QPoint& point)
{
    QString path = QString("%1/%2/%3.jpg").arg(g_path).arg(point.x()).arg(point.y());
    QPixmap image;
    if(image.load(path))
    {
        if(g_this)
        {
            emit g_this->addImage(image, point);   // 由于不能在子线程中访问ui，所以这里通过信号将图片传递到ui线程进行绘制
        }
    }
    QThread::msleep(500);
}

/**
 * @brief 加载显示瓦片图元
 */
void MapGraphicsView::loatImage()
{
    clearMapItem();
    m_itemGroup = m_mapItemGroup.value(getKey());
    if(!m_itemGroup) return;
    if(m_itemGroup->boundingRect().isEmpty())   // 如果图元为空则加载图元显示
    {
        m_imgTitle.clear();
        getTitle();
        g_path = m_path + QString("/%1").arg(getKey());
        m_future = QtConcurrent::map(m_imgTitle, readImg);
    }
    m_scene->addItem(m_itemGroup);
    m_scene->setSceneRect(m_itemGroup->boundingRect());   // 根据图元大小自适应调整场景大小
}

/**
 * @brief 移除当前显示瓦片图元
 */
void MapGraphicsView::clearMapItem()
{
    if(m_itemGroup)
    {
        m_scene->removeItem(m_itemGroup);
        m_itemGroup = nullptr;
    }
}

/**
 * @brief 清空保存瓦片图元的哈希表
 */
void MapGraphicsView::clearHash()
{
    if(m_mapItemGroup.isEmpty()) return;
    clearMapItem();     // 移除已经添加的图层
    m_keyIndex = 0;
    QList<int>keys = m_mapItemGroup.keys();
    for(auto key : keys)
    {
        QGraphicsItemGroup* item = m_mapItemGroup.value(key);
        delete item;
        m_mapItemGroup.remove(key);
    }
}

/**
 * @brief   获取当前层级的key值
 * @return  返回-1表示不存在
 */
int MapGraphicsView::getKey()
{
    if(m_mapItemGroup.isEmpty()) return -1;

    QList<int>keys = m_mapItemGroup.keys();
    std::sort(keys.begin(), keys.end());    // 由于keys不是升序的，所以需要进行排序
    if(m_keyIndex < 0 || m_keyIndex >= keys.count())
    {
        return -1;
    }
    return keys.at(m_keyIndex);
}

/**
 * @brief       绘制地图瓦片图元
 * @param img   显示的图片
 * @param pos   图片显示的位置
 */
void MapGraphicsView::on_addImage(QPixmap img, QPoint pos)
{
    if(!m_itemGroup)
    {
        return;
    }
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(img);
    QPoint& begin = m_imgTitle.first();
    item->setPos((pos.x() - begin.x()) * 256, (pos.y() - begin.y()) * 256);   // 以第一张瓦片为原点
    m_itemGroup->addToGroup(item);
    m_scene->setSceneRect(m_itemGroup->boundingRect());   // 根据图元大小自适应调整场景大小
}
