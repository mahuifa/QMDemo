#ifndef TEST1_H
#define TEST1_H

#include <QWidget>
#include "xlsxdocument.h"


namespace Ui {
class Test1;
}

class Test1 : public QWidget
{
    Q_OBJECT

public:
    explicit Test1(QWidget *parent = nullptr);
    ~Test1();

private slots:
    void on_but_open_clicked();

    void on_but_init_clicked();
    void on_but_write_clicked();

    void on_but_recv_clicked();

    void on_but_saveAs_clicked();

    void on_but_close_clicked();

private:
    Ui::Test1 *ui;

    QXlsx::Document* m_xlsx = nullptr;
};

#endif // TEST1_H
