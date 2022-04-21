#ifndef NETPROPERTY_H
#define NETPROPERTY_H

#include <QWidget>

namespace Ui {
class NetProperty;
}

class NetProperty : public QWidget
{
    Q_OBJECT

public:
    explicit NetProperty(QWidget *parent = nullptr);
    ~NetProperty();

private:

    void init();

private:
    Ui::NetProperty *ui;
};

#endif // NETPROPERTY_H
