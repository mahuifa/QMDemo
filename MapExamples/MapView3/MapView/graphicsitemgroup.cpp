/********************************************************************
 * 文件名： graphicsitemgroup.cpp
 * 时间：   2024-05-26 19:44:24
 * 开发者：  mhf
 * 邮箱：   1603291350@qq.com
 * 说明：   瓦片图元组
 * ******************************************************************/
#include "graphicsitemgroup.h"
#include "bingformula.h"

#include <qfont.h>
#include <qgraphicsscene.h>
#include <QDebug>
#include <QPen>

GraphicsItemGroup::GraphicsItemGroup(QGraphicsItem* parent)
    : QGraphicsItemGroup(parent)
{}

GraphicsItemGroup::~GraphicsItemGroup() {}

/**
 * @brief        添加绘制瓦片图
 * @param info
 */
void GraphicsItemGroup::addImage(const ImageInfo& info)
{
    quint64 key = (info.x << 24) + info.y;
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
        auto* item = new QGraphicsPixmapItem(info.img);
        QPoint pos = Bing::tileXYToPixelXY(QPoint(info.x, info.y));
        item->setPos(pos);
        this->addToGroup(item);
        m_itemsImg[key] = item;

        // 绘制边框
        auto* itemR = new QGraphicsRectItem(0, 0, 255, 255);
        itemR->setPos(pos);
        itemR->setPen(QPen(Qt::red));
        this->addToGroup(itemR);
        m_itemsR[key] = itemR;

        // 绘制瓦片编号
        QString str = QString("%1，%2").arg(info.x).arg(info.y);
        QFont font("宋体", 14);
        auto itemText = new QGraphicsSimpleTextItem(str);
        itemText->setFont(font);
        itemText->setPos(pos);
        itemText->setPen(QPen(Qt::red));
        this->addToGroup(itemText);
        m_itemsText[key] = itemText;
    }
}
