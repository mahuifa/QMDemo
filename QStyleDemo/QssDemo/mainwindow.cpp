#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include <QDebug>
#include <QToolBar>
#include <QStyle>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QToolTip>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qss样式表入门练习Demo - V%1").arg(APP_VERSION));
    init();
    initListView();
    initTreeView();
    initTableView();
    initStyle();
    connectSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->mdiArea->setBackground(Qt::NoBrush);           // 如果不设置则QMdiArea的qss样式不会生效
    ui->checkBox->setTristate(true);                   // 开启QCheckBox不确定态

    ui->pushButton_3->setAutoDefault(true);            // 设置默认按键
    ui->pushButton_4->setFlat(true);                   // 设置后按钮跟背景色融为一体
    ui->but_icon->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarMenuButton)); // 按键添加图标

    // 为QPushButton添加菜单
    QMenu* menu = new QMenu(this);
    menu->addAction(new QAction("打开", this));
    menu->addAction(new QAction("保存", this));
    ui->pushButton_5->setMenu(menu);
    // 为QToolButton添加菜单
    ui->toolButton->setMenu(menu);
    ui->toolButton_2->setMenu(menu);
    ui->toolButton_3->setMenu(menu);

    // 设置菜单栏
    QMenu* menuFile = ui->menubar->addMenu("文件");
    QAction* open = menuFile->addAction("打开");
    open->setCheckable(true);                        // 设置菜单项可选
    menuFile->addSeparator();                        // 添加分割栏
    QAction* save = menuFile->addAction("保存");
    save->setCheckable(true);                        // 设置菜单项可选
    save->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));  // 设置菜单图标
    ui->menubar->addMenu("编辑");
    ui->menubar->addMenu("构建");

    // 设置工具栏
    QToolBar* toolbar = new QToolBar(this);
    toolbar->addAction(open);
    toolbar->addAction(save);
    this->addToolBar(Qt::LeftToolBarArea, toolbar);   // 添加工具栏，停靠在窗口左侧

    // 设置状态栏
    ui->statusbar->addPermanentWidget(new QLabel("状态栏永久label"));
    ui->statusbar->showMessage("状态栏临时消息");
}

/**
 * @brief 初始化ListView
 */
void MainWindow::initListView()
{
    QStringList strList;
    strList << "123" << "456" << "789" << "abc" << "阿斯蒂芬"
            << "123" << "456" << "789" << "abc" << "阿斯蒂芬"
            << "123" << "456" << "789" << "abc" << "阿斯蒂芬";
    strModel = new QStringListModel(strList);
    ui->listView->setModel(strModel);
    ui->listView_2->setModel(strModel);
    ui->undoView->setModel(strModel);
    ui->listWidget->addItems(strList);
}

/**
 * @brief 初始化QTreeView
 */
void MainWindow::initTreeView()
{
    QFileSystemModel* model = new QFileSystemModel;   // 提供对本地文件系统的访问
    model->setRootPath(QDir::currentPath());          // 将模型正在监视的目录设置为newPath
    ui->treeView->setModel(model);
    ui->treeView_2->setModel(model);
}

/**
 * @brief 初始化QTableView
 */
void MainWindow::initTableView()
{
    QFileSystemModel* fModel = new QFileSystemModel;   // 提供对本地文件系统的访问
    fModel->setRootPath(QDir::currentPath());          // 将模型正在监视的目录设置为newPath
    ui->tableView->setModel(fModel);

    QStandardItemModel* sModel = new QStandardItemModel;
    sModel->setColumnCount(5);                         // 设置表格有5列
    sModel->setRowCount(10);                           // 设置表格有10行

    for(int i = 0; i < sModel->columnCount(); i++)
    {
        sModel->setItem(0, i, new QStandardItem(this->style()->standardIcon(QStyle::StandardPixmap(i)),   // 设置第一行表格图标
                                                QString("内容%1").arg(i)));                                // 设置第一行表格内容
        sModel->setHeaderData(i, Qt::Horizontal, QString("第%1列").arg(i));  // 设置列标题
    }
    ui->tableView_2->setModel(sModel);
    ui->tableView_2->horizontalHeader()->setSortIndicatorShown(true);
}

/**
 * @brief 加载qss文件
 */
void MainWindow::initStyle()
{
    QFile file(":/style.css");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);

        QString strQss;
        while (!stream.atEnd())
        {
            strQss.append(stream.readLine());
        }
        qApp->setStyleSheet(strQss);               // 设置整个程序的样式表而不是当前窗口
    }
    else
    {
        qWarning() << "打开qss文件失败！";
    }
}

/**
 * @brief 绑定信号槽
 */
void MainWindow::connectSlots()
{
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->progressBar, &QProgressBar::setValue);
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, ui->progressBar_2, &QProgressBar::setValue);
    connect(ui->verticalSlider, &QSlider::valueChanged, ui->progressBar_3, &QProgressBar::setValue);

    connect(ui->treeView, &QTreeView::expanded, ui->treeView_2, &QTreeView::setRootIndex);  // 可以通过设置树视图的根索引来显示特定目录的内容
}

/**
 * @brief       打印QCheckBox选中、未选中、不确定三态
 * @param arg1
 */
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    qDebug() << arg1;
}

/**
 * @brief       移除tab选项卡
 * @param index
 */
void MainWindow::on_tabWidget_2_tabCloseRequested(int index)
{
    if(index == -1) return;

    QWidget* tab = ui->tabWidget_2->widget(index);
    ui->tabWidget_2->removeTab(index);
    delete tab;
    tab = nullptr;
}
