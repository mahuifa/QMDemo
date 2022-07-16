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
