/******************************************************************************
 * @文件名     simpleudpgroup.h
 * @功能       简易的UDP组播通信Demo，主要：UDP组播不能使用connectToHost绑定地址
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/19
 * @备注
 *****************************************************************************/
#ifndef SIMPLEUDPGROUP_H
#define SIMPLEUDPGROUP_H

#include <QWidget>
#include <QUdpSocket>

namespace Ui {
class SimpleUdpGroup;
}

class SimpleUdpGroup : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleUdpGroup(QWidget *parent = nullptr);
    ~SimpleUdpGroup();

private slots:
    void on_readyRead();
    void on_but_connect_clicked();

    void on_but_send_clicked();

private:
    Ui::SimpleUdpGroup *ui;
    QUdpSocket* m_udpSocket = nullptr;         // UDP通信对象
};

#endif // SIMPLEUDPGROUP_H
