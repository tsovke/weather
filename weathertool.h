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
        QJsonParseError err;
        QJsonDocument doc=QJsonDocument::fromJson(jsonArr,&err);
        if (err.error != QJsonParseError::NoError) {
            return;
        }
        if (!doc.isObject()) {
            return;
        }
        QJsonObject cities=doc.object();
        for (auto it=cities.begin();it!=cities.end();++it) {
            mCityMap.insert(it.key(),it.value().toString());
        }
    }

public:
    WeatherTool() {}
    static QString getCityCode(QString cityName){
        if(mCityMap.isEmpty()){
            initCityMap();
        }

        QMap<QString,QString>::iterator it=mCityMap.find(cityName);
        if(it==mCityMap.end()){
            it=mCityMap.find(cityName+"市");
        }
        if(it==mCityMap.end()){
            it=mCityMap.find(cityName+"县");
        }
        if(it!=mCityMap.end()){
            return it.value();
        }
        return "";
    }
};
QMap<QString,QString> WeatherTool::mCityMap={};

#endif // WEATHERTOOL_H
