#include "titlebar.h"
#include "ui_titlebar.h"
#include <QStyle>

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);

    m_parentWidget = this->parentWidget();   // 获取标题栏父窗口
}

TitleBar::~TitleBar()
{
    delete ui;
}

/**
 * @brief   返回标题栏背景层，用于处理鼠标事件
 * @return
 */
QWidget *TitleBar::getBackground() const
{
    return ui->background;
}

/**
 * @brief         设置标题文本
 * @param title
 */
void TitleBar::setWindowTitle(const QString& title)
{
    QWidget::setWindowTitle(title);
    ui->label_title->setText(title);
}

/**
 * @brief               窗口状态改变
 * @param windowStates
 */
void TitleBar::on_windowStateChanged(Qt::WindowStates windowStates)
{
    switch (windowStates)
    {
    case Qt::WindowMaximized:
        ui->but_max->setProperty("Max", true);
        break;
    case Qt::WindowNoState:
        ui->but_max->setProperty("Max", false);
        break;
    default:break;
    }
    ui->but_max->style()->polish(ui->but_max);
}


void TitleBar::on_but_min_clicked()
{
    if(!m_parentWidget)return;
    m_parentWidget->showMinimized();
}


void TitleBar::on_but_max_clicked()
{
    if(!m_parentWidget)return;
    if(m_parentWidget->isMaximized())
    {
        m_parentWidget->showNormal();
    }
    else
    {
        m_parentWidget->showMaximized();
    }
}

void TitleBar::on_but_close_clicked()
{
    if(!m_parentWidget)return;
    m_parentWidget->close();
}

