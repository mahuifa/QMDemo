#include "mapgraphicsview.h"

#include "bingformula.h"
#include "geturl.h"
#include <qthread.h>
#include <QDebug>
#include <QFont>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <QtMath>

MapGraphicsView::MapGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    this->setScene(m_scene);
    this->setDragMode(QGraphicsView::ScrollHandDrag);   // 鼠标拖拽

    // 窗口左上角初始显示位置(中国)
    m_scenePos.setX(5700);
    m_scenePos.setY(2700);
    //    this->setMouseTracking(true);                       // 开启鼠标追踪

    connect(GetUrlInterface::getInterface(), &GetUrlInterface::update, this, &MapGraphicsView::drawImg);
}

MapGraphicsView::~MapGraphicsView() {}

void MapGraphicsView::setRect(int level)
{
    int w = int(qPow(2, level) * 256);
    QRect rect(0, 0, w, w);
    m_scene->setSceneRect(rect);

    // 将显示位置移动到缩放之前的位置
    this->horizontalScrollBar()->setValue(qRound(m_scenePos.x() - m_pos.x()));
    this->verticalScrollBar()->setValue(qRound(m_scenePos.y() - m_pos.y()));
}

/**
 * @brief       绘制瓦片图
 * @param info
 */
void MapGraphicsView::drawImg(const ImageInfo& info)
{
    if (!m_itemGroup.contains(info.z))   // 如果图层不存在则添加
    {
        auto* item = new GraphicsItemGroup();
        m_itemGroup.insert(info.z, item);
        m_scene->addItem(item);
    }

    GraphicsItemGroup* itemGroup = m_itemGroup.value(info.z);
    if (itemGroup)
    {
        itemGroup->addImage(info);
    }
}

/**
 * @brief 清空所有瓦片
 */
void MapGraphicsView::clear()
{
    auto* itemGroup = m_itemGroup.value(m_level);
    if (itemGroup)
    {
        delete itemGroup;
        m_itemGroup.remove(m_level);
        m_level = 0;
    }
}

void MapGraphicsView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);

    if (event->buttons() & Qt::LeftButton)
    {
        m_moveView = true;
    }
}

/**
 * @brief          鼠标释放
 * @param event
 */
void MapGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);

    if (m_moveView)   // 在鼠标左键释放时获取新的瓦片地图
    {
        emit mousePos(this->mapToScene(event->pos()).toPoint());
        getShowRect();
        m_moveView = false;
    }
}

/**
 * @brief        鼠标滚轮缩放
 * @param event
 */
void MapGraphicsView::wheelEvent(QWheelEvent* event)
{
    m_pos = event->pos();                          // 鼠标相对于窗口左上角的坐标
    m_scenePos = this->mapToScene(event->pos());   // 鼠标在场景中的坐标
    if (event->angleDelta().y() > 0)
    {
        m_scenePos = m_scenePos * 2;   // 放大
        m_level++;
    }
    else
    {
        m_scenePos = m_scenePos / 2;   // 缩小
        m_level--;
    }
    m_level = qBound(0, m_level, 22);                            // 限制缩放层级
    setRect(m_level);                                            // 设置缩放后的视图大小
    emit GetUrlInterface::getInterface() -> setLevel(m_level);   // 设置缩放级别
    getShowRect();

    // 隐藏缩放前所有图层
    for (auto itemG : m_itemGroup)
    {
        itemG->hide();
    }

    if (m_itemGroup.contains(m_level))   // 如果图层存在则显示
    {
        GraphicsItemGroup* itemGroup = m_itemGroup.value(m_level);
        itemGroup->show();
    }
    else   // 如果不存在则添加
    {
        auto* item = new GraphicsItemGroup();
        m_itemGroup.insert(m_level, item);
        m_scene->addItem(item);
    }
}

/**
 * @brief       窗口大小变化后获取显示新的地图
 * @param event
 */
void MapGraphicsView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    //    getShowRect();
}

/**
 * @brief       窗口显示时设置显示瓦片的视图位置
 * @param event
 */
void MapGraphicsView::showEvent(QShowEvent* event)
{
    QGraphicsView::showEvent(event);
    setRect(m_level);
}

/**
 * @brief 获取当前场景的显示范围（场景坐标系）
 */
void MapGraphicsView::getShowRect()
{
    QRect rect;
    int w = int(qPow(2, m_level) * 256);   // 最大范围
    QPoint tl = this->mapToScene(0, 0).toPoint();
    QPoint br = this->mapToScene(this->width(), this->height()).toPoint();
    rect.setX(qMax(tl.x(), 0));
    rect.setY(qMax(tl.y(), 0));
    rect.setRight(qMin(br.x(), w));
    rect.setBottom(qMin(br.y(), w));
    emit GetUrlInterface::getInterface() -> showRect(rect);
}
