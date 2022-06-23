#ifndef TEST4_H
#define TEST4_H

#include <QWidget>

namespace Ui {
class Test4;
}

class Test4 : public QWidget
{
    Q_OBJECT

public:
    explicit Test4(QWidget *parent = nullptr);
    ~Test4();

private:
    Ui::Test4 *ui;
};

#endif // TEST4_H
