#include "logconfig.h"
#include <qapplication.h>
#include <qdir.h>
#include <QSettings>

#define CONFIG_PATH QApplication::applicationDirPath() + "/config.ini"

LogConfig::LogConfig() {}

void LogConfig::init()
{
    initTxtConfig();
}

TxtConfig LogConfig::txtConfig;

void LogConfig::initTxtConfig()
{
    QFile file(CONFIG_PATH);
    QSettings config(CONFIG_PATH, QSettings::IniFormat);
    if (!file.exists())
    {
        config.beginGroup("LogConfig");
        config.setValue("relyMode", 0);
        config.setValue("time", 24);
        config.setValue("size", 100);
        config.setValue("name", "");
        config.setValue("rowNum", 1000);
        config.endGroup();
    }

    config.beginGroup("LogConfig");
    txtConfig.relyMode = (TxtRelyMode) config.value("relyMode").toUInt();
    txtConfig.time = config.value("time").toUInt();
    txtConfig.size = config.value("size").toUInt();
    txtConfig.rowNum = config.value("rowNum").toUInt();
    txtConfig.name = config.value("name").toString();
    config.endGroup();
}

void LogConfig::setTxtLogName(QString name)
{
    QSettings config(CONFIG_PATH, QSettings::IniFormat);
    config.setValue("LogConfig/name", name);
    txtConfig.name = name;
}
