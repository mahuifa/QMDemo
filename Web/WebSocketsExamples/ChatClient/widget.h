#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWebSocket>

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
    void on_connected();
    void on_disconnected();
    void on_textMessageReceived(const QString &message);

    void on_but_connect_clicked();

    void on_but_send_clicked();

private:
    Ui::Widget *ui;

    QWebSocket m_client;
};
#endif // WIDGET_H
