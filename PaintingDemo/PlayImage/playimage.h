#ifndef PLAYIMAGE_H
#define PLAYIMAGE_H

#include <QWidget>
#include <qmutex.h>

class PlayImage : public QWidget
{
    Q_OBJECT
public:
    explicit PlayImage(QWidget *parent = nullptr);

    void updateImage(const QImage& image);
    void updatePixmap(const QPixmap& pixmap);

signals:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_pixmap;
    QMutex m_mutex;
};

#endif // PLAYIMAGE_H
