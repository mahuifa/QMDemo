#include "widget.h"
#include "ui_widget.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStyle>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(600, 400);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("Qss属性功能 - V%1").arg(APP_VERSION));

    initStyle();

    qDebug() << "在构造函数中获取属性值：" << m_bgColor.name();   // 无法获取到qss修改后的属性值
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::isChecked() const
{
    return m_checked;
}

void Widget::setChecked(bool value)
{
    m_checked = value;
}

QColor Widget::isBgColor() const
{
    return m_bgColor;
}

void Widget::setBgColor(QColor color)
{
    m_bgColor = color;
}

Widget::AgeEnum Widget::age() const
{
    return m_age;
}

void Widget::setAge(AgeEnum value)
{
    m_age = value;
}

/**
 * @brief 加载qss文件
 */
void Widget::initStyle()
{
    QString strFile = qApp->applicationDirPath() + "/style.css";   // 这里我没有使用资源文件，而是把样式表文件放在当前路径下，便于随时更换
    QFile file(strFile);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);

        QString strQss;
        while (!stream.atEnd())
        {
            strQss.append(stream.readLine());
        }
        qApp->setStyleSheet(strQss);               // 设置整个程序的样式表而不是当前窗口
    }
    else
    {
        qWarning() << "打开qss文件失败！";
    }
}

/**
 * @brief 通过Q_PROPERTY定义的属性更新Qss样式
 *        设置属性的方式有两种
 *        方式一：setChecked
 *        方式二：setProperty("checked", value)  : 设置成功返回true，否则返回false
 */
void Widget::on_pushButton_clicked()
{
    this->setChecked(!this->isChecked());   // 更改控件的属性 【Q_PROPERTY】
    this->style()->polish(this);            // 属性值更改后重新初始化给定控件的样式。
}

/**
 * @brief 通过动态属性的方式更新QSS样式
 *        如果没有通过Q_PROPERTY定义属性，使用setProperty("property1", value)
 *        设置后会将property1添加为动态属性，并且返回false，
 *        效果和使用Q_PROPERTY定义的属性类似
 */
void Widget::on_pushButton_2_clicked()
{
    static bool value = true;
    qDebug() << this->setProperty("property1", value);   // 设置动态属性
    value = !value;

    this->style()->polish(this);                         // 属性值更改后重新初始化给定控件的样式。
}

/**
 * @brief 在Qss通过qproperty-属性 的方式修改属性的值，qproperty 语法只在程序启动显示控件是生效一次
 *        在构造函数中由于控件还没有开始显示，所以qproperty没生效，是无法获取修改后的属性值的，在窗口显示后就可以获取到属性值
 *        注意：虽然主要继承于QObject的类都可以通过Q_PROPERTY定义属性，但是只有继承于QWidget的类定义的属性可以通过Qss修改，
 *             因为QObject不包含QStyle
 */
void Widget::on_pushButton_3_clicked()
{
    qDebug() << "程序启动后获取属性值：" << m_bgColor.name();
    qDebug() <<"Qss设置的属性值：" << m_age;
}
