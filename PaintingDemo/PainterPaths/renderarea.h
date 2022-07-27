#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

public slots:

private:
    void initPath();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<QPainterPath> m_paths;
};

#endif // RENDERAREA_H
