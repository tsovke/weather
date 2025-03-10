#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QString>

class Today
{
public:
    Today()
    {
        date = "2025-03-03";
        city = "广州";

        ganmao = "感冒指数";

        wendu = 0;
        shidu = "0%";
        pm25 = 0;
        quality = "无数据";

        type = "多云";
        fl = "2级";
        fx = "南风";

        high = 30;
        low = 18;
    }
public:
    QString date;
    QString city;

    QString ganmao;

    int wendu;
    QString shidu;
    int pm25;
    QString quality;

    QString type;
    QString fl;
    QString fx;

    int high;
    int low;
};
class Day
{
public:
    Day()
    {
        date = "2025-03-03";
        week = "周一";

        type = "多云";
        fl = "2级";
        fx = "南风";

        high = 24;
        low = 10;
        aqi = 0;
    }
public:
    QString date;
    QString week;

    QString type;
    QString fl;
    QString fx;

    int high;
    int low;

    int aqi;
};
#endif // WEATHERDATA_H
