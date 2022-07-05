#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QWidget>
#include <opencv2/opencv.hpp>

using namespace cv;

namespace Ui {
class VideoDisplay;
}

class VideoDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit VideoDisplay(QWidget *parent = nullptr);
    ~VideoDisplay();

signals:
    void updateImage(QImage img);

private slots:
    void on_but_open_clicked();
    void readImage();
    void quitThread();
    QImage MatToQImage(const Mat& mat);

    void on_but_play_clicked();

    void on_but_file_clicked();

private:
    Ui::VideoDisplay *ui;

    VideoCapture* m_cap = nullptr;
    Mat mat;
    bool m_play = false;
};

#endif // VIDEODISPLAY_H
