#ifndef USERBACKSTAGE_H
#define USERBACKSTAGE_H

#include <QDialog>
#include <qsqltablemodel.h>

namespace Ui {
class UserBackstage;
}

class UserBackstage : public QDialog
{
    Q_OBJECT
public:
    enum UserType {
        User,            // 普通用户
        Admin,           // 普通管理员
        Root             // 超级管理员(唯一)
    };
public:
    explicit UserBackstage(QWidget *parent = nullptr);
    ~UserBackstage();

    void setUserType(UserType type);

private slots:
    void on_but_set_clicked();

    void on_but_delete_clicked();

private:
    Ui::UserBackstage *ui;
    QSqlTableModel* m_model = nullptr;                                   // 创建一个 单个数据库表的可编辑数据模型
    UserType m_userType;
};

#endif // USERBACKSTAGE_H
