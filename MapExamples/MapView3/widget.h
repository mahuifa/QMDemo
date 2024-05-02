#ifndef WIDGET_H
#define WIDGET_H

#include "geturl.h"
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
class Widget;
}

QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

private slots:
    void on_com_url_activated(int index);

    void on_com_url_editTextChanged(const QString& arg1);

    void on_but_clear_clicked();

private:
    void showRect(QRect rect);

private:
    Ui::Widget* ui;
    GetUrl* m_geturl = nullptr;
};
#endif   // WIDGET_H
