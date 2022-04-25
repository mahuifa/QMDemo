#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QChartGlobal>
#include <QAudioDeviceInfo>

QT_CHARTS_BEGIN_NAMESPACE    // QtCharts命名空间
class QLineSeries;           // 在头文件中声明要用到的QtCharts类，而不直接引入头文件
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE      // 引入QtCharts命名空间

class XYSeriesIODevice;
class QAudioInput;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initChart();         // 初始化绘制图表
    void audioSample();       // 采样绘制录音音频波形

private:
    Ui::Widget *ui;

    QAudioDeviceInfo m_inputDevice;                // 音频输入设备信息对象
    XYSeriesIODevice* m_device = nullptr;          // IO接口，用于获取音频数据并显示
    QLineSeries* m_series = nullptr;               // 折线图对象
    QAudioInput* m_audioInput = nullptr;           // 录音设备对象
};
#endif // WIDGET_H
