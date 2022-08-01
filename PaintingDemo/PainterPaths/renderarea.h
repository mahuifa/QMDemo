#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

    void setFill(bool flag);

public slots:

private:
    void initPath();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<QPainterPath> m_paths;
    QPainterPath  m_path;
    bool m_isFill = false;           // 是否使用填充
};

#endif // RENDERAREA_H
