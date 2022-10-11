#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include <qsqlquery.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void initModel();
    bool isTableExists(const QString& table);
    void on_but_connect_clicked();

    void on_but_add_clicked();

    void on_but_read_clicked();

    void on_but_user_clicked();

private:
    Ui::Widget *ui;
    QSqlTableModel* m_model = nullptr;                                   // 创建一个 单个数据库表的可编辑数据模型
    QSqlDatabase m_db;
};
#endif // WIDGET_H
