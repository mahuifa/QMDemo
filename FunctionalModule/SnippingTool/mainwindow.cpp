#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QDesktopWidget>
#include <QPushButton>
#include <QScreen>
#include <QThread>
#include <QToolBar>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置工具栏
    QAction* acNew   = new QAction("新建截图");
    QAction* acSave  = new QAction("保存截图");
    QAction* acClear  = new QAction("取消截图");
    QMenu* menu = new QMenu(this);
    menu->addAction(new QAction("全屏", this));
    menu->addAction(new QAction("矩形", this));
    menu->addAction(new QAction("窗口", this));
    m_butModel = new QToolButton();
    m_butModel->setPopupMode(QToolButton::InstantPopup);
    m_butModel->setMenu(menu);
    m_butModel->setText("模式");
    QToolBar* toolbar = new QToolBar(this);
    toolbar->addAction(acNew);
    toolbar->addWidget(m_butModel);
    toolbar->addAction(acSave);
    toolbar->addAction(acClear);
    this->addToolBar(Qt::TopToolBarArea, toolbar);   // 添加工具栏
    connect(menu, &QMenu::triggered, this, &MainWindow::on_triggered);
    connect(acNew, &QAction::triggered, this, &MainWindow::on_newGrab);
    connect(acSave, &QAction::triggered, this, &MainWindow::on_saveImage);
    connect(acClear, &QAction::triggered, this, &MainWindow::on_clearImage);


}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief         更新当前选择模式
 * @param action
 */
void MainWindow::on_triggered(QAction *action)
{
    m_butModel->setText(action->text());
}

void MainWindow::on_newGrab(bool checked)
{
    Q_UNUSED(checked)
    QString strModel = m_butModel->text();
    if(strModel == "全屏")
    {
        grabPixmap(QRect(0, 0, -1, -1));
    }
    else if(strModel == "矩形")
    {

    }
    else if(strModel == "窗口")
    {

    }
    else
    {
    }
}

/**
 * @brief       开始截图
 * @param rect
 */
void MainWindow::grabPixmap(QRect rect)
{
//    this->hide();            // 截图之前将当前窗口隐藏，避免截取的图像中包含当前窗口，这种方法很慢，需要延时等待几百毫秒，否则还是会有当前窗口

    setWindowOpacity(0);       // 最好的方法是将当前窗口设置成完全透明
    QDesktopWidget *desk = QApplication::desktop();         // 获取桌面根窗口
    QScreen * screen = QGuiApplication::primaryScreen();    // 获取默认屏幕
    m_pixmap = screen->grabWindow(desk->winId(), rect.x(), rect.y(), rect.width(), rect.height());          // 抓取屏幕图像
    ui->centralwidget->updatePixmap(m_pixmap);                     // 显示捕获的图像
//    this->show();            // 截图完成后显示窗口
    setWindowOpacity(1);
}

/**
 * @brief         保存截图
 * @param checked
 */
void MainWindow::on_saveImage(bool checked)
{
    Q_UNUSED(checked)
    if(m_pixmap.isNull()) return;

    QString strName = QFileDialog::getSaveFileName(this, "保存到", "./", "便携式网络图形(*.png);;JPEG文件(*.jpg)");
    if(strName.isEmpty()) return;

    QImage image = m_pixmap.toImage();
    if(image.save(strName))
    {
        qDebug() << "保存成功！";
    }
    else
    {
        qDebug() << "保存失败！";
    }
}

/**
 * @brief         清除截图
 * @param checked
 */
void MainWindow::on_clearImage(bool checked)
{
    Q_UNUSED(checked)
    if(m_pixmap.isNull()) return;
    m_pixmap = QPixmap();
    ui->centralwidget->updatePixmap(m_pixmap);
}
