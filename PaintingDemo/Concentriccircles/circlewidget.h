#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H

#include <QObject>
#include <QWidget>

class CircleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CircleWidget(QWidget *parent = nullptr);

    void setFloatBased(bool flag);           // 设置是否使用浮点型数据类型来保存绘制图案的数据
    void setAntialiased(bool flag);          // 是否使用抗锯齿渲染
    void setFrame(int value);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool floatBased = false;         // 是否使用浮点类型数绘制
    bool antialiased = false;        // 是否开启抗锯齿
    int frameNo = 0;
};

#endif // CIRCLEWIDGET_H
