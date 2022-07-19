/******************************************************************************
 * @文件名     renderarea.h
 * @功能      1、绘制原点和坐标轴；
 *           2、绘制线段、散点、多线段、多边形、矩形、圆角矩形、椭圆、弧线、弦形、扇形、绘制路径、文字、图片；
 *           3、可设置画笔样式、笔帽样式、线条连接样式、笔刷样式、画笔粗细、颜色、抗锯齿；
 *           4、可设置坐标原点水平、垂直偏移、旋转、缩放。
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/07/18
 * @备注
 *****************************************************************************/
#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QStyle>
#include <QPen>
class RenderArea : public QWidget
{
    Q_OBJECT
public:
    // 绘制形状
    enum Shape {
        Line,
        Points,
        Polyline,
        Polygon,
        Rect,
        RoundedRect,
        Ellipse,
        Arc,
        Chord,
        Pie,
        Path,
        Text,
        Pixmap
    };
    Q_ENUM(Shape)   // 该宏向元对象系统注册一个枚举类型，便于通过QMetaEnum获取枚举信息
public:
    explicit RenderArea(QWidget *parent = nullptr);

signals:

public slots:
    void setTranslate(qreal dx, qreal dy);
    void setAntialiased(bool antialiased);
    void setPen(const QPen& pen);
    void setBrush(const QBrush& brush);
    void setRotate(qreal rotate);
    void setShape(Shape shape);

private:
    void drawOrigin(QPainter& painter);
    void drawPattern(QPainter& painter);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QPointF m_offset;               // 坐标原点
    QPen m_pen;                     // 画笔
    QBrush m_brush;                 // 笔刷
    qreal m_rotate = 0;             // 旋转角度
    Shape m_shape = Line;           // 绘制图案
    qreal m_zoom = 1;               // 缩放级别
    QPixmap m_pixmap;               // 绘制的图片
    bool m_antialiased = false;     // 渲染是否抗锯齿
};

#endif // RENDERAREA_H
