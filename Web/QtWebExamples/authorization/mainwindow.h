#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwebenginepage.h>
class QWebEngineView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void on_featurePermissionRequested(const QUrl &securityOrigin, QWebEnginePage::Feature feature);

private:
    QWebEngineView* m_view = nullptr;
};
#endif // MAINWINDOW_H
