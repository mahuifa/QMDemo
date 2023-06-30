#include "mainwindow.h"
#include <QWebEngineView>
#include <QMap>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_view(new QWebEngineView(this))
{
    this->resize(1024, 750);          // 设置窗口大小
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QWebEnginePage权限管理 - V%1").arg(APP_VERSION));  // 设置窗口标题

    this->setCentralWidget(m_view);          // 将m_view设置为主窗口的中心小部件
    QWebEnginePage* page = m_view->page();
    connect(page, &QWebEnginePage::featurePermissionRequested, this, &MainWindow::on_featurePermissionRequested);

#if 1
    page->load(QUrl(QStringLiteral("https://app.xunjieshipin.com/luping/")));
#else
    page->load(QUrl(QStringLiteral("qrc:/video.html")));
#endif
}

MainWindow::~MainWindow()
{
}

/**
 * @brief                 当securityOrigin标识的网站请求使用功能标识的资源或设备时，会发出信号，触发当前函数
 * @param securityOrigin
 * @param feature
 */
void MainWindow::on_featurePermissionRequested(const QUrl &securityOrigin, QWebEnginePage::Feature feature)
{
    QString strFeature = "程序需要使用";
    switch (feature)
    {
    case QWebEnginePage::Notifications:
        strFeature += "Notifications";
        break;
    case QWebEnginePage::Geolocation:
        strFeature += "定位硬件或服务";
        break;
    case QWebEnginePage::MediaAudioCapture:
        strFeature += "音频捕获设备，如麦克风";
        break;
    case QWebEnginePage::MediaVideoCapture:
        strFeature += "视频设备，如摄像机";
        break;
    case QWebEnginePage::MediaAudioVideoCapture:
        strFeature += "音频和视频捕获设备";
        break;
    case QWebEnginePage::MouseLock:
        strFeature += "鼠标锁定";
        break;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))    // 后面两个枚举Qt5.10以后才有
    case QWebEnginePage::DesktopVideoCapture:
        strFeature += "视频输出捕获";
        break;
    case QWebEnginePage::DesktopAudioVideoCapture:
        strFeature += "音频和视频输出捕获";
        break;
#endif
    }
    strFeature += "，是否允许？";

    // 弹出一个弹窗
    QMessageBox msg(this);
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle("注意！");
    msg.setText(QString("%1").arg(securityOrigin.toString()));
    msg.setInformativeText(strFeature);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);

    if(msg.exec() == QMessageBox::Yes)
    {
        // 用户已授予权限
        m_view->page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
    }
    else
    {
        // 用户拒绝了权限
        m_view->page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
    }
}

