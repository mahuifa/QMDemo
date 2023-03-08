#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <qfuturewatcher.h>

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
    void on_resultReadyAt(int resultIndex);
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QFutureWatcher<quint64> m_futureWatcher;
    QList<int> m_maxs;
};
#endif // WIDGET_H
