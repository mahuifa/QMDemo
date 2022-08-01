#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

// 绘图操作类型
enum Operation {
    NoOperation,        // 没有进行操作
    Translate,          // 偏移绘图坐标原点
    Rotate,             // 以坐标原点为中心进行旋转
    Scale               // 将绘制图案按比例缩放
};

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

    void setShape(const QPainterPath& shape);
    void setOperations(const QList<Operation>& operations);

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawOutline(QPainter& painter);
    void transformPainter(QPainter& painter);
    void drawCoordinates(QPainter& painter);

public slots:

private:
    QList<Operation> m_operations;    // 保存所有的缩放、偏移、旋转操作
    QRect m_fontRectX;              // X轴字体矩形范围
    QRect m_fontRectY;              // Y轴字体矩形范围
    QPainterPath m_shape;           // 用于绘制图案的路径
};

#endif // RENDERAREA_H
