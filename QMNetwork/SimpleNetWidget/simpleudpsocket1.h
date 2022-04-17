/******************************************************************************
 * @文件名     simpleudpsocket1.h
 * @功能       简易的UDP通信功能Demo，第一种实现方式，传统UDP通信方式。
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/17
 * @备注
 *****************************************************************************/
#ifndef SIMPLEUDPSOCKET1_H
#define SIMPLEUDPSOCKET1_H

#include <QWidget>
#include <QUdpSocket>

namespace Ui {
class SimpleUdpSocket1;
}

class SimpleUdpSocket1 : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleUdpSocket1(QWidget *parent = nullptr);
    ~SimpleUdpSocket1();

private slots:
    void on_readyRead();
    void on_but_connect_clicked();

    void on_but_send_clicked();

private:
    Ui::SimpleUdpSocket1 *ui;

    QUdpSocket* m_udpSocket = nullptr;         // UDP通信对象
};

#endif // SIMPLEUDPSOCKET1_H
