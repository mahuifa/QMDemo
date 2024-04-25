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
    this->setMouseTracking(true);                       // 开启鼠标追踪

    connect(GetUrlInterface::getInterface(), &GetUrlInterface::update, this, &MapGraphicsView::drawImg);
}

MapGraphicsView::~MapGraphicsView() {}

/**
 * @brief       缩放后设置场景大小范围
 * @param rect
 */
void MapGraphicsView::setRect(QRect rect)
{
    m_scene->setSceneRect(rect);

    // 将显示位置移动到缩放之前的位置
    this->horizontalScrollBar()->setValue(qRound(m_scenePos.x() - m_pos.x()));
    this->verticalScrollBar()->setValue(qRound(m_scenePos.y() - m_pos.y()));
    getShowRect();
}

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
        m_level = info.z;
        setRect(m_level);
    }
    quint64 key = (quint64(info.z) << 48) + (info.x << 24) + info.y;
    if (m_itemsImg.contains(key))   // 如果瓦片已经存在则直接绘制
    {
        m_itemsImg[key]->setPixmap(info.img);
        m_itemsImg[key]->show();
        m_itemsR[key]->show();
        m_itemsText[key]->show();
    }
    else   // 如果瓦片不存在则添加图元
    {
        // 绘制瓦片图
        auto item = m_scene->addPixmap(info.img);
        QPoint pos = Bing::tileXYToPixelXY(QPoint(info.x, info.y));
        item->setPos(pos);
        // 绘制边框
        auto itemR = m_scene->addRect(0, 0, 255, 255, QPen(Qt::red));
        itemR->setPos(pos);
        // 绘制瓦片编号
        QString str = QString("%1，%2").arg(info.x).arg(info.y);
        QFont font("宋体", 14);
        auto itemText = m_scene->addSimpleText(str, font);
        itemText->setPos(pos);
        itemText->setPen(QPen(Qt::red));
        m_itemsImg[key] = item;
        m_itemsR[key] = itemR;
        m_itemsText[key] = itemText;
    }
}

/**
 * @brief 清空所有瓦片
 */
void MapGraphicsView::clear()
{
    m_itemsImg.clear();
    m_itemsR.clear();
    m_itemsText.clear();
    m_scene->clear();
}

/**
 * @brief        获取鼠标移动坐标
 * @param event
 */
void MapGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);

    if (event->buttons() & Qt::LeftButton)
    {
        emit mousePos(this->mapToScene(event->pos()).toPoint());
        getShowRect();
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
