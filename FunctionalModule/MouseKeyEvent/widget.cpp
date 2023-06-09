#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMetaEnum>
#include <globalmouseevent.h>
#include "globalkeyevent.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qt-自定义全局鼠标键盘事件监听Demo - V%1").arg(APP_VERSION));
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    connect(GlobalMouseEvent::getInstance(), &GlobalMouseEvent::mouseEvent, this, &Widget::on_mouseEvent);
    connect(GlobalMouseEvent::getInstance(), &GlobalMouseEvent::wheelEvent, this, &Widget::on_wheelEvent);
    connect(GlobalKeyEvent::getInstance(), &GlobalKeyEvent::keyEvent, this, &Widget::on_keyEvent);
}

Widget::~Widget()
{
#if defined(Q_OS_LINUX)
    GlobalMouseEvent::removeMouseEvent();
    GlobalKeyEvent::removeKeyEvent();
#endif
    delete ui;
}

/**
 * @brief       全局鼠标事件
 * @param event
 */
void Widget::on_mouseEvent(QMouseEvent* event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonPress:   // 鼠标按下
    {
        QString but;
        switch (event->button())
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
        case Qt::XButton1:
        {
            but = "功能键【后退】";
            break;
        }
        case Qt::XButton2:
        {
            but = "功能键【前进】";
            break;
        }
        default:
        {
            but = "未知";
            break;
        }
        }
        QString str = QString("鼠标%1按下：(x:%2, y:%3)").arg(but).arg(event->x()).arg(event->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::MouseMove:     // 鼠标移动
    {
        QString str = QString("鼠标移动：(x:%1, y:%2)").arg(event->x()).arg(event->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::MouseButtonRelease:     // 鼠标抬起
    {
        QString but;
        switch (event->button())
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
        case Qt::XButton1:
        {
            but = "功能键【后退】";
            break;
        }
        case Qt::XButton2:
        {
            but = "功能键【前进】";
            break;
        }
        default:
        {
            but = "未知";
            break;
        }
        }
        QString str = QString("鼠标%1释放：(x:%2, y:%3)").arg(but).arg(event->x()).arg(event->y());
        ui->textEdit->append(str);
        break;
    }
    default:
        break;
    }
    delete event;       // 使用完成后记得delete
}


/**
 * @brief       全局鼠标滚轮事件
 * @param event
 */
void Widget::on_wheelEvent(QWheelEvent* event)
{
    QString str = QString("鼠标滚轮：%1，(x:%2, y:%3)").arg(event->delta() > 0 ? "向前" : "向后").arg(event->x()).arg(event->y());
    ui->textEdit->append(str);
    delete event;       // 使用完成后记得delete
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
void Widget::on_keyEvent(QKeyEvent* event)
{
    QMetaEnum type = QMetaEnum::fromType<QEvent::Type>();
    QMetaEnum key = QMetaEnum::fromType<Qt::Key>();
    QMetaEnum keyboard = QMetaEnum::fromType<Qt::KeyboardModifiers>();
    QString str = QString("状态：[%1]\t按键：[%2]\t修饰：[%3]]").arg(type.valueToKey(event->type()))
                                           .arg(key.valueToKey(event->key()))
                                           .arg(QString(keyboard.valueToKeys(int(event->modifiers()))));
    if(!event->text().isEmpty())
    {
        str += QString("\t字符：[%1]").arg(event->text());
    }
    ui->textEdit->append(str);
    delete event;       // 使用完成后记得delete
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
