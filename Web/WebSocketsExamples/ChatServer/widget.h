#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QWebSocketServer>
#include <QtWebSockets>

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
    void on_newConnection();
    void on_closed();
    void on_textMessageReceived(const QString &message);
    void on_disconnected();
    void on_but_listen_clicked();

    void on_but_send_clicked();

private:
    Ui::Widget *ui;

    QWebSocketServer* m_server = nullptr;
    QList<QWebSocket*> m_clients;
};
#endif // WIDGET_H
