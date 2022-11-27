#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "globalmouseevent.h"
#include "globalkeyevent.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qt-自定义全局鼠标监听Demo - V%1").arg(APP_VERSION));
    connect(GlobalMouseEvent::getInstance(), &GlobalMouseEvent::mouseSignal, this, &Widget::on_mouseSignal);
    connect(GlobalKeyEvent::getInstance(), &GlobalKeyEvent::keyEvent, this, &Widget::on_keyEvent);
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief        获取全局鼠标事件
 * @param event
 */
void Widget::on_mouseSignal(QEvent* event)
{
    QMouseEvent* me = dynamic_cast<QMouseEvent*>(event);
    switch (event->type())
    {
    case QEvent::MouseButtonPress:   // 鼠标按下
    {
        QString but;
        switch (me->button())
        {
        case Qt::LeftButton:
        {
            but = "左键";
            break;
        }
        case Qt::RightButton:
        {
            but = "右键";
            break;
        }
        case Qt::MiddleButton:
        {
            but = "中键";
            break;
        }
        default:
        {
            but = "未知";
            break;
        }
        }
        QString str = QString("鼠标%1按下：(x:%2, y:%3)").arg(but).arg(me->x()).arg(me->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::MouseMove:     // 鼠标移动
    {
        QString str = QString("鼠标移动：(x:%1, y:%2)").arg(me->x()).arg(me->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::MouseButtonRelease:     // 鼠标右键抬起
    {
        QString but;
        switch (me->button())
        {
        case Qt::LeftButton:
        {
            but = "左键";
            break;
        }
        case Qt::RightButton:
        {
            but = "右键";
            break;
        }
        case Qt::MiddleButton:
        {
            but = "中键";
            break;
        }
        default:
        {
            but = "未知";
            break;
        }
        }
        QString str = QString("鼠标%1释放：(x:%2, y:%3)").arg(but).arg(me->x()).arg(me->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::Wheel:    // 鼠标滚轮
    {
        QWheelEvent* we = dynamic_cast<QWheelEvent*>(event);
        QString str = QString("鼠标滚轮：%1，(x:%2, y:%3)").arg(we->delta() > 0 ? "向前" : "向后").arg(we->x()).arg(we->y());
        ui->textEdit->append(str);
        break;
    }
    default:
        break;
    }

    delete event;
}

/**
 * @brief 安装全局鼠标事件监听器
 */
void Widget::on_but_mouseI_clicked()
{
    bool ret = GlobalMouseEvent::installMouseEvent();
    ui->textEdit->append(QString("<<<<<<<<<< 全局鼠标事件监听器安装%1 >>>>>>>>>>").arg(ret ? "成功" : "失败"));
}

/**
 * @brief 卸载全局鼠标事件监听器
 */
void Widget::on_but_mouser_clicked()
{
    bool ret = GlobalMouseEvent::removeMouseEvent();
    ui->textEdit->append(QString("<<<<<<<<<< 全局鼠标事件监听器卸载%1 >>>>>>>>>>").arg(ret ? "成功" : "失败"));
}

/**
 * @brief        全局键盘事件
 * @param event
 */
void Widget::on_keyEvent(QKeyEvent *event)
{
    qDebug() << Qt::Key(event->key()) << event->modifiers() << event->text();
    delete event;
}

/**
 * @brief 安装全局键盘事件监听器
 */
void Widget::on_but_keyI_clicked()
{
    bool ret = GlobalKeyEvent::installKeyEvent();
    ui->textEdit->append(QString("<<<<<<<<<< 全局键盘事件监听器安装%1 >>>>>>>>>>").arg(ret ? "成功" : "失败"));
}

/**
 * @brief 卸载全局键盘事件监听器
 */
void Widget::on_but_KeyR_clicked()
{
    bool ret = GlobalKeyEvent::removeKeyEvent();
    ui->textEdit->append(QString("<<<<<<<<<< 全局键盘事件监听器卸载%1 >>>>>>>>>>").arg(ret ? "成功" : "失败"));
}
