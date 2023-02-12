/******************************************************************************
 * @文件名     xyseriesiodevice.h
 * @功能       通过QAudioInput录制的音频数据因为需要用于绘制波形，而不是保存为音频文件，
 *             所以需要重写QIODevice类，获取录制的音频数据
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/24
 * @备注
 *****************************************************************************/
#ifndef XYSERIESIODEVICE_H
#define XYSERIESIODEVICE_H

#include <QIODevice>
#include <QChartGlobal>
#include <QVector>
#include <QElapsedTimer>
#include <QPointF>

QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class XYSeriesIODevice : public QIODevice
{
    Q_OBJECT
public:
    explicit XYSeriesIODevice(QXYSeries *series, QObject *parent = nullptr);

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    QXYSeries* m_series= nullptr;      // 用于显示音频数据的对象
    QVector<QPointF> m_buffer;
};

#endif // XYSERIESIODEVICE_H
