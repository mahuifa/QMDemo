#include "widget.h"
#include "ui_widget.h"

#include <qmessagebox.h>
#include <qsqltablemodel.h>
#include <QSqlError>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QSql-使用事务提交修改内容到数据库Demo - V%1").arg(APP_VERSION));

    m_model = new QSqlTableModel(this);
    m_model->setTable("person");         // 设置需要操作的表
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);  // 设置数据不自动保存到数据库表
    m_model->select();                   // 查询表中所有数据

    ui->tableView->setModel(m_model);
    ui->tableView->resizeColumnsToContents();   // 根据表格中的内容自动调整列宽

}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 开启sqlite事务并提交修改，相对于直接使用submitAll更有优势（开始事务可以优化写入大量数据速度）
 */
void Widget::on_but_submit_clicked()
{
    bool ret = m_model->database().transaction();
    if(!ret) return;
    if(m_model->submitAll())           // 提交所有修改，如果开启了事务，则保存到事务
    {
        m_model->database().commit();  // 如果开启了事务，则将事务保存到数据库
        QMessageBox::about(this, "注意！", "保存修改内容成功！");
    }
    else
    {
        m_model->database().rollback();
        QMessageBox::warning(this, "错误！", QString("数据提交错误：%1").arg(m_model->lastError().text()));
    }
}

/**
 * @brief 还原所有未提交的修改
 */
void Widget::on_but_revert_clicked()
{
    m_model->revertAll();
}

/**
 * @brief 退出
 */
void Widget::on_but_quit_clicked()
{
    this->close();
}

