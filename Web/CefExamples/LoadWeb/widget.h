#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "QCefSetting.h"
#include "QCefView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QCefView* cefViewWidget = nullptr;
};
#endif // WIDGET_H
