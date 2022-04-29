/******************************************************************************
 * @文件名     netproperty.h
 * @功能       使用QNetworkInterface类获取当前系统所有的网卡信息，如IP地址，物理地址，网卡类型等
 *            注意：使用的有些功能比较新，如QNetworkInterface::InterfaceType、QNetworkInterface::type() const
 *            在Qt 5.11以后才有，所以使用时需要注意自己的qt版本，如果版本过低只需删除没有的功能就可以
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/22
 * @备注
 *****************************************************************************/
#ifndef NETPROPERTY_H
#define NETPROPERTY_H

#include <QWidget>

namespace Ui {
class NetProperty;
}

class NetProperty : public QWidget
{
    Q_OBJECT

public:
    explicit NetProperty(QWidget *parent = nullptr);
    ~NetProperty();

private slots:
    void on_com_Interface_activated(int index);

private:

    void init();
    QString FlagsToQString(int flags);
    QString TypeToQString(int type);

private:
    Ui::NetProperty *ui;
};

#endif // NETPROPERTY_H
