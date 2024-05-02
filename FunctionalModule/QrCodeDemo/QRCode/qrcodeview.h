#ifndef QRCODEVIEW_H
#define QRCODEVIEW_H

#include <QImage>
#include <QWidget>

class QrCodeView : public QWidget
{
    Q_OBJECT
public:
    explicit QrCodeView(QWidget* parent = nullptr);

    void setString(const QString& str);   // 传入需要编码的字符串
    bool save(const QString& path);       // 保存生成的二维码图片到指定路径
    QImage getImage();                    // 获取生成的二维码

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

signals:

private:
    QString m_str;
    QImage m_img;       // 保存原图
    QImage m_imgShow;   // 保存缩放后的图片
};

#endif   // QRCODEVIEW_H
