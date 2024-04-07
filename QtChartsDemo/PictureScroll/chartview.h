#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <qchartview.h>
#include <QScatterSeries>
#include <qvalueaxis.h>

QT_CHARTS_USE_NAMESPACE


class ChartView : public QChartView
{
    Q_OBJECT
public:
    explicit ChartView(QWidget *parent = nullptr);
    ~ChartView() override;

    enum DataType
    {
        Type1,
        Type2,
        Type3
    };
    struct Data
    {
        QPointF pointf;
        QPixmap pixmap;
        QGraphicsPixmapItem* item = nullptr;
        DataType type;
    };

    void addData(const Data& data);
    void addData(const QPointF& point, DataType type, const QString& imgPath);

signals:
    void showLog(QString str);
    void add(Data data);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleClickedPoint(const QPointF &point);
    void on_saveData();
    void on_loadData();
    void on_reset();
    void hidePixmap();
    void on_valueChanged(int value);
    void on_showLog(QString str);


private:
    QValueAxis* m_xAxis = nullptr;
    QValueAxis* m_yAxis = nullptr;
    QScatterSeries* m_series1 = nullptr;
    QScatterSeries* m_series2 = nullptr;
    QScatterSeries* m_series3 = nullptr;
    QScrollBar* m_scrollBar = nullptr;
    QList<Data> m_datas;

    int m_minY = 0;
    int m_maxY = 100;
    int m_minReality = 0;
    int m_maxReality = 100;
    int m_start = 0;
};

#endif // CHARTVIEW_H
