#ifndef TEST4_H
#define TEST4_H

#include <QWidget>
#include "Interface.h"

namespace Ui {
class Test4;
}

class Test4 :public InterFace
{
    Q_OBJECT

public:
    explicit Test4(QWidget *parent = nullptr);
    ~Test4();

    QString getExcelName() override;

private slots:
    void on_pushButton_clicked();

private:
    Ui::Test4 *ui;
};

#endif // TEST4_H
