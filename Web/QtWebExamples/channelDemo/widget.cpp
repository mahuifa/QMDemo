#include "widget.h"
#include "ui_widget.h"

#include <qdir.h>
#include <qwebchannel.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(Core::getInstance(), &Core::webButClicked, this, &Widget::on_webButClicked);

    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject("CoreId", Core::getInstance());  // 向QWebChannel注册用于Qt和Web交互的对象。

    ui->webEngineView->page()->setWebChannel(channel);       // 将与webEngineView要使用的web通道实例设置为channel
    ui->webEngineView->setUrl(QDir("./web/webClient.html").absolutePath());
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief     显示web传来的信号
 * @param str
 */
void Widget::on_webButClicked(QString str)
{
    ui->textEdit->append(str);
}

/**
 * @brief 点击按键将信号发送给web
 */
void Widget::on_pushButton_clicked()
{
    static int i = 0;
    emit Core::getInstance()->qtButClicked(QString("Qt 按键点击 %1").arg(i++));
}
