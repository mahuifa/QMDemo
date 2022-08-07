/******************************************************************************
 * @文件名       titlebar.h
 * @功能        自定义标题栏
 *
 * @开发者     mhf
 * @时间       2022/03/26
 * @备注
 *****************************************************************************/
#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

    QWidget* getBackground() const;

public Q_SLOTS:
    void setWindowTitle(const QString& title);
    void on_windowStateChanged(Qt::WindowStates windowStates);

private slots:
    void on_but_min_clicked();

    void on_but_max_clicked();

    void on_but_close_clicked();

private:
    Ui::TitleBar *ui;
    QWidget* m_parentWidget = nullptr;
};

#endif // TITLEBAR_H
