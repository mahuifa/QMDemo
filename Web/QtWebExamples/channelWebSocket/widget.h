#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "swebchannel.h"

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
    void on_but_send_clicked();
    void on_webButClicked(QString message);

    void on_but_openWeb_clicked();

private:
    Ui::Widget *ui;
    SWebChannel m_channel;
};

/**
 * @brief  Qt和Web端交互的中介单例类
 */
class Core : public QObject
{
    Q_OBJECT
public:
    static Core* getInstance()
    {
        static Core core;
        return &core;
    }

signals:
    /**
     * @brief     Qt发送给Web的信号
     * @param str
     */
    void qtButClicked(QString str);

    /**
     * @brief     Web发送给Qt的信号
     * @param str
     */
    void webButClicked(QString str);

public slots:
    /**
     * @brief     Web端需要调用Qt槽函数来传递，必须声明为public slots，否则web找不到
     * @param str
     */
    void on_webButClicked(QString str)
    {
        emit webButClicked(str);
    }
};

#endif // WIDGET_H
