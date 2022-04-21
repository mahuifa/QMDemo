#include "netproperty.h"
#include "ui_netproperty.h"

#include <qnetworkinterface.h>

NetProperty::NetProperty(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetProperty)
{
    ui->setupUi(this);
    init();
}

NetProperty::~NetProperty()
{
    delete ui;
}

void NetProperty::init()
{
    // 获取所有网络接口
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    ui->com_Interface->clear();
    for(auto interface : interfaces)
    {
        qDebug() << interface.humanReadableName() << interface.name() << interface.type();
        ui->com_Interface->addItem(interface.humanReadableName());       // 显示所有网络接口名称
    }
}
