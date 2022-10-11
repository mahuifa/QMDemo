#include "userbackstage.h"
#include "ui_userbackstage.h"
#include <QDebug>
#include <qmessagebox.h>
#include <qsqlquery.h>

UserBackstage::UserBackstage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserBackstage)
{
    ui->setupUi(this);

    m_model = new QSqlTableModel(this);
    m_model->setTable("User");                                // 设置需要显示的数据库表
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_model->setHeaderData(0, Qt::Horizontal, "ID");
    m_model->setHeaderData(1, Qt::Horizontal, "用户名");
    m_model->setHeaderData(2, Qt::Horizontal, "密码");
    m_model->setHeaderData(3, Qt::Horizontal, "用户类型");
    ui->tableView->setModel(m_model);

    m_model->select();                                         // 获取数据库中的数据
    ui->tableView->resizeColumnsToContents();                  // 根据表格中的内容自动调整列宽
}

UserBackstage::~UserBackstage()
{
    delete ui;
}

/**
 * @brief       设置登录的用户类型
 * @param type
 */
void UserBackstage::setUserType(UserBackstage::UserType type)
{
    m_userType = type;

    // 添加用户类型
    ui->com_type->clear();
    switch (type)
    {
    case User:
    {
        break;
    }
    case Admin:
    {
        ui->com_type->addItem("普通用户", User);
        m_model->setFilter(QString("type = '%1'").arg(int(User)));     // 只显示普通用户的信息
        break;
    }
    case Root:
    {
        ui->com_type->addItem("普通用户", User);
        ui->com_type->addItem("管理员", Admin);
        break;
    }
    }
}

/**
 * @brief 设置用户信息
 */
void UserBackstage::on_but_set_clicked()
{
    QString userName = ui->line_user->text().trimmed();
    QString password = ui->line_password->text().trimmed();
    UserType userType = UserType(ui->com_type->currentIndex());
    if(userName.isEmpty())
    {
        QMessageBox::about(this, "注意！", "用户名不能为空");
        return;
    }
    if(password.isEmpty())
    {
        QMessageBox::about(this, "注意！", "用户密码不能为空");
        return;
    }

    QSqlQuery query;
    QString sql = QString("select * from User where userName = '%1';").arg(userName);  // 查询用户
    if(query.exec(sql))
    {
        if(query.next())   // true则用户存在则更新数据
        {
            int type = query.value("type").toInt();
            if(type >= m_userType)         // 如果修改的用户等级超过登录用户等级，则修改失败
            {
                return;
            }
            sql = QString("update User set password='%1', type=%2  where userName = '%3';")
                    .arg(password)
                    .arg(int(userType))
                    .arg(userName);
            query.exec(sql);
        }
        else               // 用户不存在则插入数据
        {
            query.prepare("insert into User(userName, password, type)"
                          "values (:userName, :password, :type)");
            query.bindValue(":userName", userName);
            query.bindValue(":password", password);
            query.bindValue(":type", int(userType));
            query.exec();
        }
        m_model->select();                                         // 获取数据库中的数据
    }
    else
    {
        qDebug() << "指令执行失败";
    }
}

/**
 * @brief 删除用户信息
 */
void UserBackstage::on_but_delete_clicked()
{
    QString userName = ui->line_user->text().trimmed();
    if(userName.isEmpty())
    {
        QMessageBox::about(this, "注意！", "用户名不能为空");
        return;
    }
    QSqlQuery query;
    QString sql = QString("delete from User where userName = '%1';").arg(userName);  // 查询用户
    if(query.exec(sql))
    {
        m_model->select();                                         // 获取数据库中的数据
    }
    else
    {
        qDebug() << "指令执行失败";
    }
}
