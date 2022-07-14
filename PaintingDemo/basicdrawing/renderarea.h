#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QStyle>
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

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    QByteArray m_html;
};

#endif // RENDERAREA_H
