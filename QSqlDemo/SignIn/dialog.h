#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <qsqldatabase.h>
#include "userbackstage.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    static bool loggedIn();
    static QString userName();
    static UserBackstage::UserType type();

private slots:
    bool isTableExists(const QString& table);
    void on_but_signIn_clicked();

private:
    Ui::Dialog *ui;
    QSqlDatabase m_db;
    static bool    m_loggedIn;         // 登录状态
    static QString m_userName;         // 登录的用户名
    static UserBackstage::UserType m_userType;             // 用户类型
};

#endif // DIALOG_H
