#include "renderarea.h"
#include <QDebug>
#include <qpainter.h>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    // 设置背景填充色
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);
    initPath();
}

void RenderArea::setFill(bool flag)
{
    m_isFill = flag;
}


void RenderArea::initPath()
{

    /****************** 第一行 ******************/
    // 多边形
    QPolygonF playgonf;
    playgonf.append(QPointF(20, 20));
    playgonf.append(QPointF(40, 0));
    playgonf.append(QPointF(100, 40));
    playgonf.append(QPointF(80, 80));
    playgonf.append(QPointF(40, 60));
    m_path.addPolygon(playgonf);

    // 椭圆（圆形）
    m_path.addEllipse(150, 10, 100, 100);

    // 矩形
    m_path.addRect(300, 10, 100, 60);
    // 圆形区域
    m_path.addRegion(QRegion(450, 10, 40, 50, QRegion::Ellipse));

    /****************** 第二行 ******************/
    // 圆角矩形
    m_path.addRoundRect(QRect(20, 150, 100, 100), 20, 20);

    // 添加绘制字体
    QFont font;
    font.setPixelSize(50);
    font.setBold(true);
    m_path.addText(QPoint(150, 200), font, "字体");
    // 在字体绘制起点绘制一条直线
    m_path.moveTo(QPoint(150, 200));
    m_path.lineTo(QPoint(250, 200));

    QRectF rectf(300,150,100,100);
    m_path.addRect(rectf);       // 绘制一个矩形，便于观察arcMoveTo函数效果
    //【类似moveTo函数功能】将画笔位置移动到矩形rectf中30度位置，0度为3点钟方向，正数代表逆时针，负数代表顺时针
    m_path.arcMoveTo(rectf, 30);
    QPointF pointf = m_path.currentPosition();  // 记录移动后的画笔位置
    m_path.arcTo(rectf, 90, 180); // 在rectf矩形中从90度位置开始画一个弧长为180度的弧线，画出来的弧线的起点再和arcMoveTo所移动的点相连接
    m_path.addEllipse(pointf.x() - 5, pointf.y() - 5, 10, 10);  // 绘制arcMoveToy移动后的画笔绘制

    // 添加子路径
    QPainterPath  path;
    path.moveTo(450, 150);
    path.lineTo(500, 200);
    path.lineTo(460, 220);
//    path.closeSubpath();      // 如果不关闭路径则path为一个折线，关闭路径后为一个三角形
#if 1
    m_path.addPath(path);
#else
    m_path.moveTo(550, 150);
    m_path.lineTo(570, 200);
    m_path.connectPath(path);  // connectPath方法会将m_path的画笔位置(最后一个点)和path的画笔位置(第一个点)连接起来
#endif

    /****************** 第三行 ******************/
    // 三次贝塞尔曲线
    m_path.moveTo(20, 300);
    m_path.cubicTo(QPointF(130, 300), QPointF(50, 350), QPointF(100, 400));

    // 两个路径填充区域相交位置【And】
    QPainterPath path1;
    path1.addRect(150, 300, 100, 100);
    QPainterPath path2;
    path2.addEllipse(200, 280, 80, 80);
    m_path.addPath(path1.intersected(path2));   // 返回path1路径填充区域与path2路径填充区域相交的路径（功能与 【&】 相同）

    // 二次贝塞尔曲线
    m_path.moveTo(300, 300);
    m_path.quadTo(QPointF(400, 300), QPointF(360, 400));

    // 将路径中最后一个元素（点）的位置设置为QPointF(200, 0)
//    m_path.setElementPositionAt(m_path.elementCount() - 1, 200, 0);


    // 使用simplified()简化（融合）路径，合并所有相交的子路径，并返回不包含相交边的路径。连续的平行线也将合并
    QPainterPath path3;
    path3.addRect(450, 300, 60, 60);
    path3.addEllipse(480, 300, 50, 50);
    path3.setFillRule(Qt::WindingFill);         // 这里需要设置为WindingFill填充才能简化路径
    qDebug() << "简化路径后始终使用默认的填充规则：" <<path3.simplified().fillRule();
    m_path.addPath(path3.simplified());


    /****************** 第四行 ******************/
    // path5路径【减】去path4路径的填充面积
    QPainterPath path4;
    path4.addRect(20, 450, 100, 100);
    QPainterPath path5;
    path5.addEllipse(70, 430, 80, 80);
    m_path.addPath(path4.subtracted(path5));   // 功能与 【-】 相同

    // 交换path5、path6两个路径内容
    QPainterPath path6;
    path6.addRect(150, 450, 100, 100);
    path5.swap(path6);
    m_path.addPath(path5);

    // 路径反向、转换为多边形并进行变换
    QPainterPath path7 = m_path.toReversed();                 // 创建并返回路径的反向副本(原来路径不变)。
    QMatrix matrix;
    matrix.translate(150, 0);                                 // 向右偏移150像素
    m_path.addPolygon(path7.toFillPolygons(matrix).at(0));    // 将m_path反向副本转换为多个多边形，并取出第1个（原来的最后一个）,并通过matrix进行变化

    // 返回一条路径，该路径是path8路径的填充区域和path9的填充区域的并集【加】。
    QPainterPath path8;
    path8.addRect(450, 450, 60, 60);
    QPainterPath path9;
    path9.addEllipse(480, 450, 50, 50);
    m_path.addPath(path8.united(path9));                      // united()函数的功能与 【+ |】 相同

//    m_path.translate(200, 0);                               // 将当前路径整体进行偏移

    /****************** 打印信息 ******************/
    qDebug() << "绘制路径中元素的个数："         << m_path.elementCount();
    qDebug() << "获取第0个元素："              << m_path.elementAt(0);            // 相当于获取playgonf.append(QPointF(20, 20));这一个点
    qDebug() << "画家填充规则："               << m_path.fillRule();
    qDebug() << "传入矩形是否与路径m_path相交：" << m_path.intersects(QRect(150, 10, 100, 100));
    qDebug() << "传入路径是否与路径m_path相交：" << m_path.intersects(path1);
    qDebug() << "路径中是否没有元素："          << m_path.isEmpty();
    qDebug() << "当前路径的长度："             << path1.length();
    qDebug() << "指定长度len占整个路径百分比："  << path1.percentAtLength(100);
    qDebug() << "路径path1中25%位置的点："     << path1.pointAtPercent(0.25);
    // 通过slopeAtPercent计算路径中50%位置的斜率，这里由于路径只有一条直线，所以斜率是 (50 - 0) / (100 - 0) = 0.5
    QPainterPath pathSlope;
    pathSlope.moveTo(0, 0);
    pathSlope.lineTo(100, 50);
    qDebug() << "路径中50%位置斜率：" <<pathSlope.slopeAtPercent(0.5);

    qDebug() << "toFillPolygon："<< m_path.toFillPolygons().count();          // 仅为具有重叠边界矩形的子路径创建多边形
    qDebug() << "toSubpathPolygons："<< m_path.toSubpathPolygons().count();   // 此函数为每个子路径创建一个多边形，而不考虑相交的子路径（即重叠的边界矩形）
}


void RenderArea::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
//    painter.setBrush(Qt::cyan);                     // 设置笔刷

    if(!m_isFill)
    {
        painter.drawPath(m_path);
//        painter.drawPath(m_path.translated(100, 0));    // 将当前路径拷贝一份并整体进行偏移，原来路径不变
//        painter.drawPolygon(m_path.toFillPolygon());  // 将路径转换为一个多边形，toFillPolygons()将路径转换为多个多边形
    }
    else
    {
        painter.fillPath(m_path, Qt::blue);
    }
//    painter.drawRect(m_path.boundingRect());     // 将绘制路径m_path的边界矩形返回并绘制，便于观察
    painter.drawRect(m_path.controlPointRect());   // 功能与boundingRect()相似
}
