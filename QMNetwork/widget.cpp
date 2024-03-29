#include "widget.h"
#include "ui_widget.h"

#include <QSharedPointer>
#include <qdebug.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("Qt网络编程Demo - V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    on_pushButton_clicked();
    delete ui;
}


/**
 * @brief 关闭并释放所有界面
 */
void Widget::on_pushButton_clicked()
{
    if(m_netProperty)
    {
        delete m_netProperty;
        m_netProperty = nullptr;
    }

    // 简易版
    while (m_simpleTcpClients.count())
    {
        delete m_simpleTcpClients.first();
        m_simpleTcpClients.removeFirst();
    }
    while (m_simpleTcpServers.count())
    {
        delete m_simpleTcpServers.first();
        m_simpleTcpServers.removeFirst();
    }
    while (m_simpleUdpSocket1s.count())
    {
        delete m_simpleUdpSocket1s.first();
        m_simpleUdpSocket1s.removeFirst();
    }
    while (m_simpleUdpSocket2s.count())
    {
        delete m_simpleUdpSocket2s.first();
        m_simpleUdpSocket2s.removeFirst();
    }
    while (m_simpleUdpGroups.count())
    {
        delete m_simpleUdpGroups.first();
        m_simpleUdpGroups.removeFirst();
    }

    // 进阶版
    while (m_tcpClients.count())
    {
        delete m_tcpClients.first();
        m_tcpClients.removeFirst();
    }
    while (m_tcpServers.count())
    {
        delete m_tcpServers.first();
        m_tcpServers.removeFirst();
    }
    while (m_udpsockets.count())
    {
        delete m_udpsockets.first();
        m_udpsockets.removeFirst();
    }
}


/**
 * @brief 打开任意多个TCP客户端界面
 */
void Widget::on_but_TCPClient_clicked()
{
    m_tcpClients.append(new TCPClient);
    m_tcpClients.last()->show();
}

/**
 * @brief 打开容易多个TCP服务端界面
 */
void Widget::on_pushButton_2_clicked()
{
    m_tcpServers.append(new TCPServer);
    m_tcpServers.last()->show();
}


void Widget::on_pushButton_3_clicked()
{
    m_udpsockets.append(new UdpSocket);
    m_udpsockets.last()->show();
}

/**
 * @brief 打开简易版本TCP客户端
 */
void Widget::on_but_simpleTcpClient_clicked()
{
    m_simpleTcpClients.append(new SimpleTcpClient);
    m_simpleTcpClients.last()->show();
}

/**
 * @brief 打开简易版本TCP服务端
 */
void Widget::on_but_simpleTcpServer_clicked()
{
    m_simpleTcpServers.append(new SimpleTcpServer);
    m_simpleTcpServers.last()->show();
}

/**
 * @brief 打开简易版UDP单播通信Demo1
 */
void Widget::on_but_simpleUdp_clicked()
{
    m_simpleUdpSocket1s.append(new SimpleUdpSocket1);
    m_simpleUdpSocket1s.last()->show();
}

/**
 * @brief 打开简易版UDP单播通信Demo2
 */
void Widget::on_but_simpleUdp_2_clicked()
{
    m_simpleUdpSocket2s.append(new SimpleUdpSocket2);
    m_simpleUdpSocket2s.last()->show();
}

/**
 * @brief 打开简易版UDP组播通信Demo
 */
void Widget::on_but_simpleUdpGroup_clicked()
{
    m_simpleUdpGroups.append(new SimpleUdpGroup);
    m_simpleUdpGroups.last()->show();
}

/**
 * @brief 打开网络属性窗口
 */
void Widget::on_but_property_clicked()
{
    if(!m_netProperty)
    {
        m_netProperty = new NetProperty();
    }
    m_netProperty->show();
}

