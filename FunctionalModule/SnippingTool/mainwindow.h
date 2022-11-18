#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtoolbutton.h>
#include "screenrect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void on_triggered(QAction *action);
    void on_newGrab(bool checked = false);
    void on_saveImage(bool checked = false);
    void on_clearImage(bool checked = false);
    void on_selectRect(QRect rect);
    void grabPixmap(QRect rect);            // 捕获图像

private:
    Ui::MainWindow *ui;
    QToolButton* m_butModel = nullptr;
    QPixmap m_pixmap;                    // 保存截取的图像
    ScreenRect m_screenRect;
};
#endif // MAINWINDOW_H
