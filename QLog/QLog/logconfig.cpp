#include "logconfig.h"
#include <QSettings>
#include <qdir.h>

LogConfig::LogConfig()
{

}

void LogConfig::init()
{
    initTxtConfig();
}

TxtConfig LogConfig::txtConfig;
void LogConfig::initTxtConfig()
{
    QFile file("./config.ini");
    QSettings config("./config.ini", QSettings::IniFormat);
    if(!file.exists())
    {
        config.beginGroup("LogConfig");
        config.setValue("relyMode", 0);
        config.setValue("time", 24);
        config.setValue("size", 100);
        config.setValue("name", "");
        config.setValue("rowNum",1000);
        config.endGroup();
    }

    config.beginGroup("LogConfig");
    txtConfig.relyMode = (TxtRelyMode)config.value("relyMode").toUInt();
    txtConfig.time = config.value("time").toUInt();
    txtConfig.size = config.value("size").toUInt();
    txtConfig.rowNum = config.value("rowNum").toUInt();
    txtConfig.name = config.value("name").toString();
    config.endGroup();
}

void LogConfig::setTxtLogName(QString name)
{
    QSettings config("./config.ini", QSettings::IniFormat);
    config.setValue("LogConfig/name", name);
    txtConfig.name = name;
}
