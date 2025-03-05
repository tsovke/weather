#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMouseEvent>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "weatherdata.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void contextMenuEvent(QContextMenuEvent* event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    //获取天气数据,解析JSON,更新UI
    void getWeatherInfo(QString cityName);
    void parseJson(QByteArray &responseData);
    void updateUI();

    //重写父类的eventfilter方法
    bool eventFilter(QObject* watched,QEvent *event)override;

    //绘制高温低温曲线
    void paintHighCurve();
    void paintLowCurve();

private slots:
    void on_btnSearch_clicked();

private:
    void onReplied(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QMenu *mExitMenu; // 右键退出的菜单
    QAction *mExitAct; // 退出的行为 - 菜单项

    bool mDragging; //
    QPoint mOffset;//窗口移动时，鼠标与左上角的偏移


    //http请求
    QNetworkAccessManager* mNetAccessManager;

    //今天和6天的天气
    Today mToday;
    Day mDay[6];

    //星期和日期
    QList<QLabel*> mWeekList;
    QList<QLabel*> mDateList;

    //天气和天气图标
    QList<QLabel*> mTypeList;
    QList<QLabel*> mTypeIconList;

    //空气污染指数
    QList<QLabel*> mAqiList;

    //风向和风力
    QList<QLabel*> mFxList;
    QList<QLabel*> mFlList;

    QMap<QString,QString> mTypeMap;

};
#endif // MAINWINDOW_H
