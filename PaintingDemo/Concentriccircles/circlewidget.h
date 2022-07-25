#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H

#include <QObject>
#include <QWidget>

class CircleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CircleWidget(QWidget *parent = nullptr);

    void setFloatBased(bool flag);
    void setAntialiased(bool flag);
    void setFrame(int value);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool floatBased = false;
    bool antialiased = false;
    int frameNo = 0;
};

#endif // CIRCLEWIDGET_H
