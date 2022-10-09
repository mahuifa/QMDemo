#include "netproperty.h"
#include "ui_netproperty.h"

#include <QMetaEnum>
#include <qnetworkinterface.h>
#include <QDeadlineTimer>

NetProperty::NetProperty(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetProperty)
{
    ui->setupUi(this);

    this->setWindowTitle("Qt使用QNetworkInterface获取网卡信息、IP地址Demo");
    // 表格自适应
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);                 // 设置如何将标题大小调整为给定模式所述大小的约束。
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);     // 然后设置要根据内容使用宽度的列

    init();
}

NetProperty::~NetProperty()
{
    delete ui;
}

/**
 * @brief 获取所有的网卡可读名称
 */
void NetProperty::init()
{
    // 获取所有网络接口
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    ui->com_Interface->clear();
    for(const QNetworkInterface& interface : interfaces)
    {
        ui->com_Interface->addItem(interface.humanReadableName(), interface.name());       // 显示所有网络接口名称
    }
}

/**
 * @brief        通过下列框选择网卡名，显示选择网卡的所有信息
 * @param index
 */
void NetProperty::on_com_Interface_activated(int index)
{
    Q_UNUSED(index)
    QString strName = ui->com_Interface->currentData().toString();
    QNetworkInterface interface =  QNetworkInterface::interfaceFromName(strName);   // 根据选择名称获取网卡

    if(!interface.isValid())   // 判断接口有效
    {
        return;
    }
    ui->tableWidget->item(0, 1)->setText(QString::number(interface.index()));                     // 获取网卡索引
#if (QT_VERSION >= QT_VERSION_CHECK(5,11,0))        // qt5.11以后版本才有
    ui->tableWidget->item(1, 1)->setText(QString::number(interface.maximumTransmissionUnit()));   // 获取网卡最大传输数据单元
#endif
    ui->tableWidget->item(2, 1)->setText(interface.name());                                       // 获取网卡名称

#if 0    // 直接显示标志枚举
    QNetworkInterface::InterfaceFlags flags = interface.flags();
    QMetaEnum metaEnum = QMetaEnum::fromType<QNetworkInterface::InterfaceFlags>();
    ui->tableWidget->item(3, 1)->setText(metaEnum.valueToKeys(flags));                            // 返回与此网络接口关联的标志
#else
    ui->tableWidget->item(3, 1)->setText(FlagsToQString(interface.flags()));
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,11,0))        // qt5.11以后版本才有
    ui->tableWidget->item(4, 1)->setText(TypeToQString(interface.type()));                        // 获取网络类型说明
#endif
    ui->tableWidget->item(5, 1)->setText(interface.hardwareAddress());                            // // 获取MAC地址

    ui->textEdit_ip->clear();
    // 如果只是获取所有的IP地址可以使用allAddresses()函数
    const QList<QNetworkAddressEntry> entrys = interface.addressEntries();  // 返回此接口拥有的 IP 地址列表及其关联的网络掩码和广播地址。
    for(auto entery : entrys)
    {
        QString strType;
        switch (entery.ip().protocol())       // 判断IP地址类型
        {
        case QAbstractSocket::IPv4Protocol:
            strType = "--------IPv4地址--------";
            break;
        case QAbstractSocket::IPv6Protocol:
            strType = "--------IPv6地址--------";
            break;
        case QAbstractSocket::AnyIPProtocol:
            strType = "--------IPv4或IPv6地址--------";
            break;
        case QAbstractSocket::UnknownNetworkLayerProtocol:
            strType = "--------未知地址--------";
            break;
        }
        ui->textEdit_ip->append(strType);             // 显示IP地址类型
        QString ipInfo = QString("IP地址：%1，子网掩码：%2，广播地址：%3").arg(entery.ip().toString())
                .arg(entery.netmask().toString())
                .arg(entery.broadcast().toString());
        ui->textEdit_ip->append(ipInfo);              // 显示IP地址信息
    }
}

/**
 * @brief        将网卡关联标志转换为可读的说明信息
 * @param flags
 * @return
 */
QString NetProperty::FlagsToQString(int flags)
{
    QString strFlags;
    if(flags & QNetworkInterface::IsUp)
    {
        strFlags += "网络接口处于活动状态";
    }
    if(flags & QNetworkInterface::IsRunning)
    {
        strFlags.append(strFlags.isEmpty() ? "" : " | ");
        strFlags += "网络接口已分配资源";
    }
    if(flags & QNetworkInterface::CanBroadcast)
    {
        strFlags.append(strFlags.isEmpty() ? "" : " | ");
        strFlags += "网络接口工作在广播模式";
    }
    if(flags & QNetworkInterface::IsLoopBack)
    {
        strFlags.append(strFlags.isEmpty() ? "" : " | ");
        strFlags += "网络接口是一个环回接口";
    }
    if(flags & QNetworkInterface::IsPointToPoint)
    {
        strFlags.append(strFlags.isEmpty() ? "" : " | ");
        strFlags += "网络接口是一个点对点接口";
    }
    if(flags & QNetworkInterface::CanMulticast)
    {
        strFlags.append(strFlags.isEmpty() ? "" : " | ");
        strFlags += "网络接口支持组播";
    }
    return strFlags;
}


/**
 * @brief        返回网卡类型说明
 * @param type   网卡类型枚举
 * @return       网卡类型说明
 */
QString NetProperty::TypeToQString(int type)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,11,0))        // qt5.11以后版本才有
    switch (type)
    {
    case QNetworkInterface::Loopback: return "虚拟环回接口，分配了环回 IP 地址 (127.0.0.1, ::1)";
    case QNetworkInterface::Virtual:  return "一种确定为虚拟的接口类型，但不是任何其他可能的类型";
    case QNetworkInterface::Ethernet: return "IEEE 802.3 以太网接口";
    case QNetworkInterface::Slip:     return "串行线路互联网协议接口";
    case QNetworkInterface::CanBus:   return "ISO 11898 控制器局域网总线接口";
    case QNetworkInterface::Ppp:      return "点对点协议接口，通过较低的传输层（通常通过无线电或物理线路串行）在两个节点之间建立直接连接";
    case QNetworkInterface::Fddi:     return "ANSI X3T12 光纤分布式数据接口，一种光纤局域网";
    case QNetworkInterface::Wifi:     return "IEEE 802.11 Wi-Fi 接口";         // 别名 Ieee80211
    case QNetworkInterface::Phonet:   return "使用 Linux Phonet socket系列的接口，用于与蜂窝调制解调器通信";
    case QNetworkInterface::Ieee802154: return "IEEE 802.15.4 个人区域网络接口，6LoWPAN 除外";
    case QNetworkInterface::SixLoWPAN:  return "6LoWPAN（低功耗无线个人区域网络上的 IPv6）接口，通常用于网状网络";
    case QNetworkInterface::Ieee80216:  return "IEEE 802.16 无线城域网";
    case QNetworkInterface::Ieee1394:   return "IEEE 1394 接口（又名“FireWire”）";
    case QNetworkInterface::Unknown:    return "接口类型无法确定或不是其他列出的类型之一";
    default:return "未知";
    }
#else
    Q_UNUSED(type)
#endif
}

