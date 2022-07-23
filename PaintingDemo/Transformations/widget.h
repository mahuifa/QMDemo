#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class RenderArea;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initUI();
    void setupShapes();

private slots:
    void shapeSelected(int index);
    void operationChanged(int index);

private:
    enum {NumTransformendAreas = 3};    // 定义一个常量，等同于宏常量【枚举常量可设置私有】

    QComboBox* m_comShape = nullptr;    // 绘制图案选择
    QComboBox* m_comOperation[NumTransformendAreas];  // 包含多个QComboBox指针的指针数组
    QList<QPainterPath> m_shapes;       // 绘制图案
    RenderArea* m_originalRenderArea = nullptr;       // 原始渲染区域
    RenderArea* m_transformedRenderAreas[NumTransformendAreas];  // 可进行缩放、偏移、旋转的区域
};
#endif // WIDGET_H
