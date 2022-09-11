#include "widget.h"
#include "ui_widget.h"
#include "xyseriesiodevice.h"
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QtCharts>
#include <qdebug.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("QtCharts绘图-动态数据的绘制（麦克风输入）Demo - V%1").arg(APP_VERSION));
    initChart();
    audioSample();
}

Widget::~Widget()
{
    m_audioInput->stop();    // 停止录制
    m_device->close();       // 关闭IO设备
    delete ui;
}

/**
 * @brief 初始化绘制图表
 */
void Widget::initChart()
{
    m_inputDevice = QAudioDeviceInfo::defaultInputDevice();   // 获取默认的音频输入设备(麦克风)

    QChart* chart = ui->chartView->chart();             // 这里直接获取QChartView包含的chart，不自己创建
    chart->legend()->hide();                            // 隐藏折线图的图例
    chart->setTitle(QString("来自麦克风的数据：%1")        // 设置图表的标题和输入设备名称
                    .arg(m_inputDevice.deviceName()));

    m_series = new QLineSeries();                       // 创建一个折线图对象
    chart->addSeries(m_series);

    QValueAxis *axisX = new QValueAxis;                 // 创建X轴
    axisX->setRange(0, 10000);                          // 设置显示同时10000个音频数据点
    axisX->setLabelFormat("%g");
    axisX->setTitleText("采样");
    QValueAxis *axisY = new QValueAxis;                 // 创建Y轴
    axisY->setRange(0, 255);                            // 因为显示的使uchar类型数据，所以Y轴设置为0~255就可以
    axisY->setTitleText("输入音频电平");

    chart->addAxis(axisX, Qt::AlignBottom);           // 在chart中添加创建的X轴，指定在图表底部
    m_series->attachAxis(axisX);                      // 将X轴关联到创建的折线图上
    chart->addAxis(axisY, Qt::AlignLeft);             // 添加创建的Y轴，指定在左侧
    m_series->attachAxis(axisY);
    m_series->setUseOpenGL(true);                     // 开启OpenGL加速，仅QLineSeries和QScatterSeries支持使用OpenGL进行加速

}

/**
 * @brief 采样音频波形显示
 */
void Widget::audioSample()
{
    QAudioFormat audio;                               // 存储音频流参数信息的对象
    audio.setSampleRate(8000);                        // 设置音频采样率
    audio.setChannelCount(1);                         // 设置通道数
    audio.setSampleSize(8);                           // 设置样本大小
    audio.setCodec("audio/pcm");                      // 设置编解码器
    audio.setByteOrder(QAudioFormat::LittleEndian);   // 采用小端模式
    audio.setSampleType(QAudioFormat::UnSignedInt);   // 设置样本数据类型

    m_audioInput = new QAudioInput(m_inputDevice, audio, this);  // 打开音频录音设备
    m_device = new XYSeriesIODevice(m_series, this);             // 将录制的音频数据输出到XYSeriesIODevice
    m_device->open(QIODevice::WriteOnly);
    m_audioInput->start(m_device);                               // 开始录制
}
