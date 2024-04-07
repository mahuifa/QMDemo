#include "widget.h"
#include "ui_widget.h"

#include <QDateTime>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(600, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QT下载瓦片地图简单示例--V%1").arg(APP_VERSION));

    m_dThread = new DownloadThread();      // 不能指定父对象
    m_dThreads = new DownloadThreads();
    connect(m_dThread, &DownloadThread::finished, this, &Widget::finished);
    connect(m_dThreads, &DownloadThreads::finished, this, &Widget::finished);
    ui->textEdit->document()->setMaximumBlockCount(100);   // 最大显示行数
    ui->textEdit->moveCursor(QTextCursor::End);            // 自动滚动到底部

    qRegisterMetaType<QList<ImageInfo>>("QList<ImageInfo>");
    qRegisterMetaType<ImageInfo>("ImageInfo");
}

Widget::~Widget()
{
    delete m_dThread;
    delete m_dThreads;
    delete ui;
}

/**
 * @brief 获取下载路径
 */
void Widget::on_but_savePath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "下载路径",
                                               qApp->applicationDirPath(),
                                               QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(path.isEmpty()) return;
    ui->line_savePath->setText(path);
}

/**
 * @brief          单线程下载瓦片地图
 * @param checked
 */
void Widget::on_but_thread_clicked(bool checked)
{
    if(checked)
    {
        m_timer.start();
        ui->progressBar->setValue(0);
        ui->but_thread->setText("停止下载");

        const QList<ImageInfo> & infos = ui->mapInput->getInputInfo();
        if(infos.isEmpty())
        {
            ui->but_thread->setText("单线程下载");
            ui->but_thread->setChecked(false);
            return;
        }
        emit m_dThread->getImage(infos);    // 开始下载
        ui->progressBar->setMaximum(infos.count());
    }
    else
    {
        ui->but_thread->setText("单线程下载");
        ui->but_thread->setChecked(false);
        m_dThread->quit();
    }
}

/**
 * @brief          多线程下载瓦片地图
 * @param checked
 */
void Widget::on_but_threads_clicked(bool checked)
{
    if(checked)
    {
        m_timer.start();
        ui->progressBar->setValue(0);
        ui->but_threads->setText("停止下载");

        const QList<ImageInfo> & infos = ui->mapInput->getInputInfo();
        if(infos.isEmpty())   // 如果列表为空则不下载
        {
            ui->but_threads->setText("多线程下载");
            ui->but_threads->setChecked(false);
            return;
        }
        m_dThreads->getImage(infos);       // 开始下载
        ui->progressBar->setMaximum(infos.count());
    }
    else
    {
        ui->but_threads->setText("多线程下载");
        ui->but_threads->setChecked(false);
        m_dThreads->quit();
    }
}

/**
 * @brief        处理下载完成的图片
 * @param index  已经下载的索引
 */
void Widget::finished(ImageInfo info)
{
    if(!info.img.isNull())   // 下载失败图片为空
    {
        // 创建文件夹
        QString strPath = ui->line_savePath->text();
        if(strPath.isEmpty())
        {
            strPath = qApp->applicationDirPath() + "/map";   // 默认保存地址
        }
        strPath += QString("/%1/%2/").arg(info.z).arg(info.x);
        QDir dir;
        dir.mkpath(strPath);
        // 保存文件
        strPath += QString("%1.%2").arg(info.y).arg(info.format);
        info.img.save(strPath);
        ui->textEdit->append(info.url);
    }
    else
    {
        ui->textEdit->append("下载失败：" + info.url);
    }

    ui->progressBar->setValue(ui->progressBar->value() + 1);
    if(ui->progressBar->value() == ui->progressBar->maximum())
    {
        ui->but_thread->setChecked(false);
        ui->but_threads->setChecked(false);
        ui->but_thread->setText("单线程下载");
        ui->but_threads->setText("多线程下载");
        qDebug() << "下载时长：" << m_timer.elapsed() <<" ms";
    }
}
