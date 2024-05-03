#include "mapgraphicsview.h"

#include "bingformula.h"
#include "geturl.h"
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
    if (m_level != info.z)
    {
        if (!m_itemGroup.contains(info.z))
        {
            auto* item = new GraphicsItemGroup();
            m_itemGroup.insert(info.z, item);
            m_scene->addItem(item);
        }
        m_level = info.z;
        setRect(m_level);
    }

    GraphicsItemGroup* itemGroup = m_itemGroup.value(m_level);
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
        emit this->zoom(true);
    }
    else
    {
        m_scenePos = m_scenePos / 2;   // 缩小
        emit this->zoom(false);
    }
}

void MapGraphicsView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    getShowRect();
}

/**
 * @brief 获取当前场景的显示范围（场景坐标系）
 */
void MapGraphicsView::getShowRect()
{
    QRect rect;
    rect.setTopLeft(this->mapToScene(0, 0).toPoint());
    rect.setBottomRight(this->mapToScene(this->width(), this->height()).toPoint());
    emit GetUrlInterface::getInterface() -> showRect(rect);
}
