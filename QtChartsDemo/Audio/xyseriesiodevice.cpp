#include "xyseriesiodevice.h"
#include <QLineSeries>
#include <qdebug.h>
#include <qvalueaxis.h>

XYSeriesIODevice::XYSeriesIODevice(QXYSeries *series, QObject *parent) :
    QIODevice(parent),
    m_series(series)
{
    const QList<QAbstractAxis*> axiss = m_series->attachedAxes();   // 获取关联的X轴
    for(auto axis : axiss)
    {
        if(axis->alignment() == Qt::AlignBottom)  // 判断是否是X轴
        {
            QValueAxis * axisX = (QValueAxis *)m_series->attachedAxes().at(0);
            int count = axisX->max() - axisX->min();   // 获取轴范围
            m_buffer.reserve(count);                   // 初始化绘制点的数组大小
            for(int i = 0; i < count; i++)
            {
                m_buffer.append(QPointF(i, 127));
            }
        }
    }
}

/**
 * @brief          readData是纯虚函数，需要重写才能实例化
 * @param data
 * @param maxlen
 * @return
 */
qint64 XYSeriesIODevice::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)
    return -1;
}

/**
 * @brief        重写IO接口，获取输入设备录制的音频数据
 * @param data   音频数据
 * @param len    数据长度
 * @return
 */
qint64 XYSeriesIODevice::writeData(const char *data, qint64 len)
{
    if(m_buffer.isEmpty()) return -1;                        // 如果未初始化数组则不显示

    static const int resolution = 4;                         // 每四个数据显示一个
    const int availableSamples = int(len) / resolution;      // 需要显示的数据个数
    int start = 0;
    if(availableSamples < m_buffer.count())                  // 如果需要显示的数据个数 < 数组个数，则每次的数据都需要往前移动
    {
        start = m_buffer.count() - availableSamples;         // 需要向前移动的数据个数
        for(int i = 0; i < start; i++)
        {
            m_buffer[i].setY(m_buffer.at(i + availableSamples).y());
        }
    }

    for(int i = start; i < m_buffer.count(); i++)           // 每隔4个取出1个数据加入绘制的数组
    {
        m_buffer[i].setY(uchar(*data));
        data += resolution;
    }

    m_series->replace(m_buffer);                           // 绘制数组中的数据
    return len;
}
