#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
class QSqlRelationalTableModel;
class QItemSelectionModel;
class QDataWidgetMapper;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void setupModel();

private slots:
    void updateButtons(int row);

private:
    Ui::Widget *ui;
    QSqlRelationalTableModel* m_model = nullptr;
    QItemSelectionModel* m_selectionModel = nullptr;
    QDataWidgetMapper* m_mapper = nullptr;
    int m_index = 0;
};
#endif // WIDGET_H
