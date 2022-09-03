#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QGraphicsScene>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initToolBar();
    void drawLine();
    void drawRect();
    void drawEllipse();
    void drawPolygon();
    void drawSimpleText();
    void drawText();
    void drawPixmap();
    void drawPath();
    void addWidget();
    void drawPoints();
    void clear();

private:
    Ui::MainWindow *ui;

    QButtonGroup m_butGroup;
    QGraphicsScene m_scene;
};
#endif // MAINWINDOW_H
