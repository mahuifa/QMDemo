#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "readthread.h"
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

private slots:
    void on_but_open_clicked();

    void on_playState(ReadThread::PlayState state);

private:
    Ui::Widget *ui;

    PlayImage* playImage = nullptr;
    ReadThread* m_readThread = nullptr;
};
#endif // WIDGET_H
