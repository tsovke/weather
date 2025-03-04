#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H

#include <QMap>
#include <QFile>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>


class WeatherTool
{
private:
    static QMap<QString, QString> mCityMap;
    static void initCityMap()
    {
        // 1. 读取文件
        QString filePath = "://res/citycode.json";
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray jsonArr = file.readAll();
        file.close();

        //2. 解析并写入到mCityMap
    }

public:
    WeatherTool() {}
    static QString getCityCode(){return {};}
};
QMap<QString,QString> WeatherTool::mCityMap={};

#endif // WEATHERTOOL_H
