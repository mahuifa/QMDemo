#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);
    ~RenderArea() override;

    void setTransform(const QTransform &transform);

private:
    void showData();

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:

private:
    QTransform m_transform;
};

#endif // RENDERAREA_H
