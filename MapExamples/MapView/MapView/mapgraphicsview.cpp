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
    this->setDragMode(QGraphicsView::ScrollHandDrag);      // 设置鼠标拖拽
//    QThreadPool::globalInstance()->setMaxThreadCount(1);   // 可以设置线程池线程数
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

    if(m_future.isRunning())   // 判断是否在运行
    {
        return;
    }
    if(event->angleDelta().y() > 0)   // 放大
    {
        if(m_keyIndex < m_mapItemGroups.count() -1)
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

    clearReset();    // 加载新瓦片路径时将之前的内容清空

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
            if(!m_mapItemGroups.contains(level))  // 如果不包含
            {
                // 初始化加载所有瓦片层级到场景中，默认不显示
                QGraphicsItemGroup* itemMap = new QGraphicsItemGroup();
                m_scene->addItem(itemMap);
                itemMap->setVisible(false);
                m_mapItemGroups[level] = itemMap;
                // 初始化加载所有瓦片层级网格到场景中，默认不显示
                QGraphicsItemGroup* itemGrid = new QGraphicsItemGroup();
                m_scene->addItem(itemGrid);
                itemGrid->setVisible(false);
                m_gridItemGroups[level] = itemGrid;
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
//    QThread::msleep(50);     // 加载时加上延时可以更加清晰的看到加载过程
}

/**
 * @brief 加载显示瓦片图元
 */
void MapGraphicsView::loatImage()
{
    quit();                  // 加载新瓦片之前判断是否还有线程在运行
    m_imgTitle.clear();
    if(m_mapitemGroup)
    {
        m_mapitemGroup->setVisible(false);        // 隐藏图层
        m_griditemGroup->setVisible(false);       // 隐藏图层
    }
    m_mapitemGroup = m_mapItemGroups.value(getKey());
    m_griditemGroup = m_gridItemGroups.value(getKey());
    if(!m_mapitemGroup || !m_griditemGroup) return;
    if(m_mapitemGroup->boundingRect().isEmpty())   // 如果图元为空则加载图元显示
    {
        getTitle();      // 获取新层级的所有瓦片编号
        g_path = m_path + QString("/%1").arg(getKey());
        m_future = QtConcurrent::map(m_imgTitle, readImg);
    }
    m_mapitemGroup->setVisible(true);              // 显示新瓦片图层
    m_griditemGroup->setVisible(true);             // 显示新网格图层
    m_scene->setSceneRect(m_mapitemGroup->boundingRect());   // 根据图元大小自适应调整场景大小
}

/**
 * @brief 清除重置所有内容
 */
void MapGraphicsView::clearReset()
{
    if(m_mapItemGroups.isEmpty()) return;
    m_keyIndex = 0;
    m_mapitemGroup = nullptr;
    m_griditemGroup = nullptr;
    m_imgTitle.clear();
    QList<int>keys = m_mapItemGroups.keys();
    for(auto key : keys)
    {
        // 清除瓦片图元
        QGraphicsItemGroup* item = m_mapItemGroups.value(key);
        m_scene->removeItem(item);    // 从场景中移除图元
        delete item;
        m_mapItemGroups.remove(key);   // 从哈希表中移除图元

        // 清除网格
        item = m_gridItemGroups.value(key);
        m_scene->removeItem(item);     // 从场景中移除图元
        delete item;
        m_gridItemGroups.remove(key);   // 从哈希表中移除图元
    }
}

/**
 * @brief   获取当前层级的key值
 * @return  返回-1表示不存在
 */
int MapGraphicsView::getKey()
{
    if(m_mapItemGroups.isEmpty()) return -1;

    QList<int>keys = m_mapItemGroups.keys();
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
    if(!m_mapitemGroup || m_imgTitle.isEmpty())
    {
        return;
    }

    // 计算瓦片显示位置，默认为256*256的瓦片大小
    QPoint& begin = m_imgTitle.first();
    int x = (pos.x() - begin.x()) * 256;
    int y = (pos.y() - begin.y()) * 256;
    // 绘制瓦片
    QGraphicsPixmapItem* itemImg = new QGraphicsPixmapItem(img);
    itemImg->setPos(x, y);   // 以第一张瓦片为原点
    m_mapitemGroup->addToGroup(itemImg);

    // 绘制网格、
    QGraphicsRectItem* itemRect = new QGraphicsRectItem(x, y, 256, 256);
    m_griditemGroup->addToGroup(itemRect);
    itemRect->setPen(QPen(Qt::red));
    // 绘制编号
    QString text = QString("%1,%2,%3").arg(pos.x()).arg(pos.y()).arg(getKey());
    QGraphicsSimpleTextItem* itemText = new QGraphicsSimpleTextItem(text);
    QFont font;
    font.setPointSize(14);                           // 设置字体大小为12
    QFontMetrics metrics(font);
    qreal w = metrics.horizontalAdvance(text) / 2.0; // 计算字符串宽度
    qreal h = metrics.height() / 2.0;               // 字符串高度
    itemText->setPos(x + 128 - w, y + 128 - h);     // 编号居中显示
    itemText->setFont(font);
    itemText->setPen(QPen(Qt::red));
    m_griditemGroup->addToGroup(itemText);

    m_scene->setSceneRect(m_mapitemGroup->boundingRect());   // 根据图元大小自适应调整场景大小
}
