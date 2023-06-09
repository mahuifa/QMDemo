#include "widget.h"
#include "ui_widget.h"
#include "QWindow"
#include <QDebug>
#include <QFile>
#include <QStyle>


Widget::Widget(QWidget *parent)
    : MWidgetBase(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();
    connectSlot();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    loadStyle();
    this->setWindowTitle(QString("自定义窗口 - V%1").arg(APP_VERSION));
    this->setTitleBar(ui->titleBar->getBackground());   // 设置标题栏
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
}

void Widget::connectSlot()
{
    connect(this, &MWidgetBase::windowStateChanged, ui->titleBar, &TitleBar::on_windowStateChanged);
}

/**
 * @brief 加载样式表
 */
void Widget::loadStyle()
{
    QFile file(":/Style/main.css");
    if (file.open(QFile::ReadOnly))
    {
        //用QTextStream读取样式文件不用区分文件编码 带bom也行
        QStringList list;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line;
            in >> line;
            list << line;
        }

        file.close();
        QString qss = list.join("\n");
        this->setStyleSheet("");
        qApp->setStyleSheet(qss);
    }
}

/**
 * @brief       隐藏Widget设置标题文本功能，添加设置标题栏文本功能
 * @param title
 */
void Widget::setWindowTitle(const QString &title)
{
    QWidget::setWindowTitle(title);
    ui->titleBar->setWindowTitle(title);
}

