#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
#include <QWidget>
#include "playimage.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void on_timerout();

private:
    Ui::Widget *ui;

    QTimer m_timer;
    QPixmap m_pixmap[2];
    QList<PlayImage*> m_playImages;
};
#endif // WIDGET_H
