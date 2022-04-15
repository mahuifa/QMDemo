#ifndef TEST2_H
#define TEST2_H

#include <QWidget>
#include "xlsxdocument.h"

namespace Ui {
class Test2;
}

class Test2 : public QWidget
{
    Q_OBJECT

public:
    explicit Test2(QWidget *parent = nullptr);
    ~Test2();

private slots:
    void on_but_open_clicked();

    void on_but_querySheet_clicked();

    void on_but_addSheet_clicked();

    void on_but_insert_clicked();

    void on_but_select_clicked();

    void on_but_rename_clicked();

    void on_but_close_clicked();

    void on_but_copy_clicked();

    void on_but_move_clicked();

    void on_but_delete_clicked();

private:
    Ui::Test2 *ui;

    QXlsx::Document* m_xlsx = nullptr;
};

#endif // TEST2_H
