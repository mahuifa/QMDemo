#ifndef WORK2_H
#define WORK2_H

#include <QObject>

class Work2 : public QObject
{
    Q_OBJECT
public:
    explicit Work2(QObject *parent = nullptr);
    ~Work2();

signals:
    void start();
    void updateData(int num);

public slots:
    void delayCount();

};

#endif // WORK2_H
