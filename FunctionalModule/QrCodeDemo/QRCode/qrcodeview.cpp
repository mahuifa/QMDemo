#include "qrcodeview.h"
#include "qrencode.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QPainter>

QrCodeView::QrCodeView(QWidget* parent)
    : QWidget{parent}
{}

void QrCodeView::setString(const QString& str)
{
    if (str.isEmpty())
        return;
    if (m_str == str)
        return;
    m_str = str;

    QRcode* qrcode = QRcode_encodeString(str.toStdString().data(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
    if (!qrcode)   // 错误返回null
    {
        qInfo() << errno;   // 打印错误码
        return;
    }
    if (qrcode->width < 20)
        return;

    int w = qMin(this->width(), this->height());   // 控件长宽的最小值，保证绘制完整正方形
    int qw = qrcode->width;
    int border = 30;                               // 二维码的边框宽度
    qreal size = (w - (border * 2)) / qreal(qw);   // 二维码小格子的长宽

    m_img = QImage(w, w, QImage::Format_ARGB32);
    QPainter painter(&m_img);
    painter.setBrush(Qt::white);   // 设置白色笔刷
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, w, w);   // 绘制w x w的白色背景
    painter.setBrush(Qt::black);    // 设置黑色笔刷用于绘制方块

    QRectF rect(0, 0, size, size);   // 创建一个黑色方块大小的矩形
    for (qint32 y = 0; y < qw; y++)
    {
        for (qint32 x = 0; x < qw; x++)
        {
            unsigned char b = qrcode->data[(y * qw) + x];
            if (b & 0x01)   // 1=黑、0=白
            {
                rect.moveTo(border + (x * size), border + (y * size));   // 将矩形移动到指定位置
                painter.drawRect(rect);                                  // 绘制黑色格子
            }
        }
    }
    delete qrcode;
    qrcode = nullptr;

    m_imgShow = m_img;
    this->update();   // 绘制图片
}

/**
 * @brief       保存生成的二维码图片到指定路径
 * @param path  保存路径 + 名称.jpg
 * @return   true 保存成功
 */
bool QrCodeView::save(const QString& path)
{
    if (path.isEmpty() || m_img.isNull())
        return false;

    QFileInfo info(path);
    if (info.suffix() == "jpg" || info.suffix() == "JPG")   // 只支持保存为jpg格式
    {
        QDir().mkpath(info.path());   // 创建路径，防止路径不存在
        return m_img.save(path);
    }
    else
    {
        return false;
    }
}

/**
 * @brief   返回生成的二维码图片
 * @return
 */
QImage QrCodeView::getImage()
{
    return m_img;
}

/**
 * @brief          在控件中显示二维码
 * @param event
 */
void QrCodeView::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    int x = (this->width() - m_imgShow.width()) / 2;
    int y = (this->height() - m_imgShow.height()) / 2;
    painter.drawImage(x, y, m_imgShow);
}

/**
 * @brief        窗口大小调整时缩放图片
 * @param event
 */
void QrCodeView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (m_img.isNull())
        return;
    int w = qMin(this->width(), this->height());   // 控件长宽的最小值，保证绘制完整正方形
    m_imgShow = m_img.scaled(w, w, Qt::KeepAspectRatio);

    this->update();
}
