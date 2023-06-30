#include "widget.h"
#include "ui_widget.h"
#include <QJsonDocument>
#include <QTime>
#include <qdir.h>
#include <qwebchannel.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("使用QtWebChannel实现Qt与Web通信交互（2），进阶功能 - V%1").arg(APP_VERSION));  // 设置窗口标题

    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject("CoreId", Core::getInstance());  // 向QWebChannel注册用于Qt和Web交互的对象。

    ui->webEngineView->page()->setWebChannel(channel);       // 将与webEngineView要使用的web通道实例设置为channel
    ui->webEngineView->setUrl(QDir("./web2/webClient.html").absolutePath());

    // 绑定槽函数，接收web界面中javascript传递过来的信号
    connect(Core::getInstance(), &Core::toQtBool, this, &Widget::on_toQtBool);
    connect(Core::getInstance(), &Core::toQtDouble, this, &Widget::on_toQtDouble);
    connect(Core::getInstance(), &Core::toQtString, this, &Widget::on_toQtString);
    connect(Core::getInstance(), &Core::toQtJsonArray, this, &Widget::on_toQtJsonArray);
    connect(Core::getInstance(), &Core::toQtJsonObject, this, &Widget::on_toQtJsonObject);
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief       显示发送的数据
 * @param str
 */
void Widget::showText(QString str)
{
    ui->textEdit->append(QString("发送：%1").arg(str));
}

/**
 * @brief       显示Web发送给Qt的数据
 * @param value
 */
void Widget::on_toQtBool(bool value)
{
    ui->textEdit->append(QString("Bool类型数据：%1").arg(value ? "true" : "false"));
}

void Widget::on_toQtDouble(double value)
{
    ui->textEdit->append(QString("double类型数据：%1").arg(value));
}

void Widget::on_toQtString(QString value)
{
    ui->textEdit->append(QString("QString类型数据：%1").arg(value));
}

void Widget::on_toQtJsonArray(QJsonArray value)
{
    QJsonDocument doc;
    doc.setArray(value);
    ui->textEdit->append(QString("QJsonArray类型数据：%1").arg(doc.toJson().data()));
}

void Widget::on_toQtJsonObject(QJsonObject value)
{
    QJsonDocument doc;
    doc.setObject(value);
    ui->textEdit->append(QString("QJsonArray类型数据：%1").arg(doc.toJson().data()));
}


/**
 * @brief 发送Bool类型数据
 */
void Widget::on_but_bool_clicked()
{
    static bool value = true;
    value = !value;
    emit Core::getInstance()->toWebBool(value);

    showText(QString("%1").arg(value ? "true" : "false"));
}

/**
 * @brief 发送double类型数据
 */
void Widget::on_but_double_clicked()
{
    double value = QTime::currentTime().msec() / 50.0;
    emit Core::getInstance()->toWebDouble(value);

    showText(QString("%1").arg(value));
}

/**
 * @brief 发送字符串类型数据
 */
void Widget::on_but_str_clicked()
{
    double value = QTime::currentTime().msec() / 50.0;
    emit Core::getInstance()->toWebString(QString("%1").arg(value));

    showText(QString("%1").arg(value));
}

/**
 * @brief 发送Json数组类型数据
 */
void Widget::on_but_array_clicked()
{
    double value = QTime::currentTime().msec() / 50.0;
    QJsonArray array = {value, value / 10.0, QString("%1").arg(value / 20.0)};
    emit Core::getInstance()->toWebJsonArray(array);

    QJsonDocument doc;
    doc.setArray(array);
    showText(QString("%1").arg(doc.toJson().data()));
}

/**
 * @brief 发送json对象类型数据
 */
void Widget::on_but_object_clicked()
{
    double value = QTime::currentTime().msec() / 50.0;
    QJsonObject obj;
    obj.insert("key1", value);
    obj.insert("key2", value / 30.0);
    obj.insert("key3", QString("%1").arg(value / 40.0));
    emit Core::getInstance()->toWebJsonObject(obj);

    QJsonDocument doc;
    doc.setObject(obj);
    showText(QString("%1").arg(doc.toJson().data()));
}

/**
 * @brief        带返回值的槽函数，将返回值传递给javascript
 * @param value
 * @return
 */
QString Core::on_returnValue(int value)
{
    qDebug() << "调用Qt槽函数，并返回值";
    return QString("调用成功：%1").arg(value);
}
