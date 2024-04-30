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

signals:

private:
    QString m_str;
    QImage m_img;
};

#endif   // QRCODEVIEW_H
