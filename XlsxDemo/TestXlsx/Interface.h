#ifndef INTERFACE_H
#define INTERFACE_H

#include <qwidget.h>


class InterFace : public QWidget
{
    Q_OBJECT

public:
    explicit InterFace(QWidget *parent = nullptr) :QWidget(parent){}
    virtual ~InterFace(){}
    /**
     * @brief   返回当前类中用到的Excel的名称
     * @return
     */
    virtual QString getExcelName() = 0;
};

#endif // INTERFACE_H
