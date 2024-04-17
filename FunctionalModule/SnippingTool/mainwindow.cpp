#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QThread>
#include <QToolBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Qt-截图工具 - V%1").arg(APP_VERSION));
    qApp->setStyleSheet("*{font: 9pt '宋体'; }");
    this->resize(800, 600);

    // 设置工具栏
    QAction* acNew = new QAction(QIcon(":/img/clipping.ico"), "新建截图");
    QAction* acSave = new QAction(QIcon(":/img/save.ico"), "保存截图");
    QAction* acClear = new QAction(QIcon(":/img/cancel.ico"), "取消截图");
    m_acModel = new QAction(QIcon(":/img/precinct.ico"), "截图模式");
    QMenu* menu = new QMenu(this);
    menu->addAction(new QAction("全屏", this));
    menu->addAction(new QAction("矩形", this));
    menu->addAction(new QAction("窗口", this));
    QToolBar* toolbar = new QToolBar(this);
    m_acModel->setMenu(menu);
    toolbar->addAction(acNew);
    toolbar->addAction(m_acModel);
    toolbar->addAction(acSave);
    toolbar->addAction(acClear);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->addToolBar(Qt::TopToolBarArea, toolbar);   // 添加工具栏
    connect(menu, &QMenu::triggered, this, &MainWindow::on_triggered);
    connect(acNew, &QAction::triggered, this, &MainWindow::on_newGrab);
    connect(acSave, &QAction::triggered, this, &MainWindow::on_saveImage);
    connect(acClear, &QAction::triggered, this, &MainWindow::on_clearImage);
    connect(&m_screenRect, &ScreenRect::selectRect, this, &MainWindow::on_selectRect);
    connect(&m_windowRect, &WindowRect::selectRect, this, &MainWindow::on_selectRect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief         更新当前选择模式
 * @param action
 */
void MainWindow::on_triggered(QAction* action)
{
    m_acModel->setText(action->text());
}

void MainWindow::on_newGrab(bool checked)
{
    Q_UNUSED(checked)
    QString strModel = m_acModel->text();
    if (strModel == "全屏")
    {
        grabPixmap(QRect(0, 0, -1, -1));
    }
    else if (strModel == "矩形")
    {
        this->hide();
        m_screenRect.show();
    }
    else if (strModel == "窗口")
    {
        this->hide();
        m_windowRect.show();
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
#if defined(Q_OS_WIN)
    setWindowOpacity(0);                                                                             // 最好的方法是将当前窗口设置成完全透明
    QDesktopWidget* desk = QApplication::desktop();                                                  // 获取桌面根窗口
    QScreen* screen = QGuiApplication::primaryScreen();                                              // 获取默认屏幕
    m_pixmap = screen->grabWindow(desk->winId(), rect.x(), rect.y(), rect.width(), rect.height());   // 抓取屏幕图像
    ui->centralwidget->updatePixmap(m_pixmap);                                                       // 显示捕获的图像
    setWindowOpacity(1);
#elif defined(Q_OS_LINUX)
    // linux下setWindowOpacity设置透明后截图还可以看到一个透明的边框，效果不是很好，所以使用hide
    this->hide();   // 截图之前将当前窗口隐藏，避免截取的图像中包含当前窗口，这种方法很慢，需要延时等待几百毫秒，否则还是会有当前窗口
    QThread::msleep(300);
    QDesktopWidget* desk = QApplication::desktop();                                                  // 获取桌面根窗口
    QScreen* screen = QGuiApplication::primaryScreen();                                              // 获取默认屏幕
    m_pixmap = screen->grabWindow(desk->winId(), rect.x(), rect.y(), rect.width(), rect.height());   // 抓取屏幕图像
    ui->centralwidget->updatePixmap(m_pixmap);                                                       // 显示捕获的图像
    this->show();                                                                                    // 截图完成后显示窗口
#endif
}

/**
 * @brief         保存截图
 * @param checked
 */
void MainWindow::on_saveImage(bool checked)
{
    Q_UNUSED(checked)
    if (m_pixmap.isNull())
        return;

    // linux下getSaveFileName不会返回默认文件后缀，所以需要在文件名中添加后缀，否则QImage::save无法通过后缀推测出文件类型，就会保存失败
    QString name = QString("%1.png").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"));
    QString strName = QFileDialog::getSaveFileName(this, "保存到", name, "便携式网络图形(*.png);;JPEG文件(*.jpg)");
    if (strName.isEmpty())
        return;

    QImage image = m_pixmap.toImage();
    if (image.save(strName))
    {
        qDebug() << "保存成功！";
    }
    else
    {
        QMessageBox::warning(this, "注意！", "文件保存失败，请检查有没有输入文件后缀名。");
    }
}

/**
 * @brief         清除截图
 * @param checked
 */
void MainWindow::on_clearImage(bool checked)
{
    Q_UNUSED(checked)
    if (m_pixmap.isNull())
        return;
    m_pixmap = QPixmap();
    ui->centralwidget->updatePixmap(m_pixmap);
}

/**
 * @brief        矩形选择截图
 * @param rect
 */
void MainWindow::on_selectRect(QRect rect)
{
    this->show();
    grabPixmap(rect);
}
