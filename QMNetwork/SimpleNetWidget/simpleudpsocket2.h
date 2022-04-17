/******************************************************************************
 * @文件名     simpleudpsocket1.h
 * @功能       简易的UDP通信功能Demo，第二种实现方式，采用类似tcp连接目标IP的方法。
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/17
 * @备注
 *****************************************************************************/
#ifndef SIMPLEUDPSOCKET2_H
#define SIMPLEUDPSOCKET2_H

#include <QUdpSocket>
#include <QWidget>

namespace Ui {
class SimpleUdpSocket2;
}

class SimpleUdpSocket2 : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleUdpSocket2(QWidget *parent = nullptr);
    ~SimpleUdpSocket2();

private slots:
    void on_readyRead();
    void on_connected();
    void on_but_connect_clicked();

    void on_but_send_clicked();

private:
    Ui::SimpleUdpSocket2 *ui;
    QUdpSocket* m_udpSocket = nullptr;         // UDP通信对象
};

#endif // SIMPLEUDPSOCKET2_H
