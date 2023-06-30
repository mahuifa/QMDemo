#include "widget.h"
#include "ui_widget.h"
#include <QDesktopServices>
#include <qdir.h>
#include <qurl.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("使用QWebSocket客户端作为QWebChannel通信数据传输接口 - V%1").arg(APP_VERSION));  // 设置窗口标题

    connect(Core::getInstance(), &Core::webButClicked, this, &Widget::on_webButClicked);

    m_channel.registerObject("CoreId", Core::getInstance());   // 向QWebChannel注册用于Qt和Web交互的对象。
    m_channel.listen(QHostAddress::AnyIPv4, 1234);             // 设置WebSocket服务端开始监听

//    ui->webEngineView->page()->setWebChannel(&m_channel);       // 将与webEngineView要使用的web通道实例设置为channel（使用QWebSocket作为数据传输接口可以不需要这一步）
    ui->webEngineView->setUrl(QDir("./web3/channelClient.html").absolutePath());
}

Widget::~Widget()
{
    delete ui;
}

/**
 * 发送数据
 */
void Widget::on_but_send_clicked()
{
    QString str = ui->lineEdit->text().trimmed();
    if(str.isEmpty()) return;
    emit Core::getInstance()->qtButClicked(str);
}

/**
 * @brief         显示接收到的web端信息
 * @param message
 */
void Widget::on_webButClicked(QString message)
{
    ui->textEdit->append(message);
}

/**
 * @brief 点击一次打开一个Web程序
 */
void Widget::on_but_openWeb_clicked()
{
    QDesktopServices::openUrl(QDir("./web3/channelClient.html").absolutePath());
}
