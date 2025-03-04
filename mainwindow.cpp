#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,mDragging(false)
    ,mNetAccessManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);//设置窗口无边框
    setFixedSize(width(),height()); //设置固定窗口大小

    //构建右键菜单
    mExitMenu =new QMenu(this);
    mExitAct =new QAction();

    //右键菜单 “退出”和图标
    mExitAct->setText("退出");
    mExitAct->setIcon(QIcon(":/res/close.png"));
    //将退出项添加到菜单
    mExitMenu->addAction(mExitAct);
    //信号槽
    connect(mExitAct,&QAction::triggered,this,[=]{
        // QApplication::quit();
        qApp->exit(0);
    });

    //将控件添加的控件数组中
    //星期和日期
    mWeekList << ui->lblWeek0 << ui->lblWeek1 << ui->lblWeek2 << ui->lblWeek3 << ui->lblWeek4
              << ui->lblWeek5;
    mDateList << ui->lblDate0 << ui->lblDate2 << ui->lblDate2 << ui->lblDate3 << ui->lblDate4
              << ui->lblDate5;
    //天气和天气图标
    mTypeList << ui->lblType0 << ui->lblType1 << ui->lblType2 << ui->lblType3 << ui->lblType4
              << ui->lblType5;
    mTypeIconList << ui->lblTypeIcon0 << ui->lblTypeIcon1 << ui->lblTypeIcon2 << ui->lblTypeIcon3 << ui->lblTypeIcon4
              << ui->lblTypeIcon5;
    //空气污染指数
    mAqiList << ui->lblAqi0 << ui->lblAqi1 << ui->lblAqi2 << ui->lblAqi3 << ui->lblAqi4
                  << ui->lblAqi5;
    //风向和风力
    mFxList << ui->lblFx0 << ui->lblFx1 << ui->lblFx2 << ui->lblFx3 << ui->lblFx4
             << ui->lblFx5;
    mFlList << ui->lblFl0 << ui->lblFl1 << ui->lblFl2 << ui->lblFl3 << ui->lblFl4
             << ui->lblFl5;

    // mNetAccessManager=new QNetworkAccessManager(this);
    connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&MainWindow::onReplied);

    getWeatherInfo("101010100");


}

MainWindow::~MainWindow()
{
    delete ui;
}

//重写虚函数
//父类中的默认实现，是忽略右键菜单事件
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    //弹出右键菜单
    mExitMenu->exec(QCursor::pos());
    event->accept();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mDragging = true;
        mOffset = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mDragging && (event->buttons() & Qt::LeftButton)) {
        QPoint new_pos = event->globalPosition().toPoint() - mOffset;
        move(new_pos);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mDragging = false;
        event->accept();
    }
}

void MainWindow::getWeatherInfo(QString cityCode)
{
    // QUrl url("http://t.weather.itboy.net/api/weather/city/"+cityCode);
    qDebug()<<cityCode;
    QUrl url("https://www.ithome.com/");

    mNetAccessManager->get(QNetworkRequest(url));
}

void MainWindow::parseJson(QByteArray &responseData)
{
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(responseData,&err);
    if(err.error!=QJsonParseError::NoError){
        return;
    }

    QJsonObject rootObj=doc.object();
    qDebug()<<rootObj.value("message").toString();

   //1. 解析日期和城市
    mToday.date=rootObj.value("date").toString();
    mToday.city= rootObj.value("cityInfo").toObject().value("city").toString();

    //2. 解析昨天的数据
    QJsonObject objData=rootObj.value("data").toObject();
    QJsonObject objYersterday=objData.value("yesterday").toObject();

    mDay[0].week=objYersterday.value("week").toString();
    mDay[0].date=objYersterday.value("ymd").toString();
    mDay[0].type=objYersterday.value("type").toString();

    QString s;
    s=objYersterday.value("high").toString().split(" ").at(1);// 高温 18℃中的"18℃"
    s=s.left(s.length()-1); //18
    mDay[0].high=s.toInt();
    QString t;
    t=objYersterday.value("low").toString().split(" ").at(1);// 低温 10℃中的"10℃"
    t=t.left(s.length()-1); //10
    mDay[0].low=t.toInt();

    mDay[0].fl=objYersterday.value("fl").toString();
    mDay[0].fx=objYersterday.value("fx").toString();

    //污染指数
     mDay[0].aqi=objYersterday.value("aqi").toInt();

    //3. 解析forecast中5天的数据
     QJsonArray forecastArr=objData.value("forecast").toArray();

     for (int i = 0; i < 5; ++i) {
         QJsonObject objForecast=forecastArr[i].toObject();

         mDay[i+1].week=objForecast.value("week").toString();
         mDay[i+1].date=objForecast.value("ymd").toString();
         mDay[i+1].type=objForecast.value("type").toString();

         QString s;
         s=objForecast.value("high").toString().split(" ").at(1);// 高温 18℃中的"18℃"
         s=s.left(s.length()-1); //18
         mDay[i+1].high=s.toInt();
         QString t;
         t=objForecast.value("low").toString().split(" ").at(1);// 低温 10℃中的"10℃"
         t=t.left(s.length()-1); //10
         mDay[i+1].low=t.toInt();

         mDay[i+1].fl=objForecast.value("fl").toString();
         mDay[i+1].fx=objForecast.value("fx").toString();

         //污染指数
         mDay[i+1].aqi=objForecast.value("aqi").toInt();
     }

    //4. 解析今天的数据
     // ganmao="感冒指数";

     // wendu=0;
     // shidu="0%";
     // pm25=0;
     // quality="无数据";

     // type="多云";
     // fl="2级";
     // fx="南风";

     // high=30;
     // low=18;
     mToday.ganmao=objData.value("ganmao").toString();
     mToday.wendu=objData.value("wendu").toInt();
     mToday.shidu=objData.value("shidu").toString();
     mToday.pm25=objData.value("pm25").toInt();
     mToday.quality=objData.value("quality").toString();
     mToday.type=mDay[1].type;
     mToday.fl=mDay[1].fl;
     mToday.fx=mDay[1].fx;
     mToday.high=mDay[1].high;
     mToday.low=mDay[1].low;

}


void MainWindow::onReplied(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug()<<"status code : "<<status_code;
        qDebug()<<"url : "<<reply->url();

        // 获取所有原始响应头的名字
        QList<QByteArray> rawHeaders = reply->rawHeaderList();

        qDebug() << "Raw headers:";
        for (const QByteArray &headerName : rawHeaders) {
            QByteArray headerValue = reply->rawHeader(headerName);
            qDebug() << headerName << ":" << headerValue;
        }

        // 处理响应数据
        QByteArray responseData = reply->readAll();
        qDebug() << "Response data:" << responseData.data();
        // 解析数据
        parseJson(responseData);
    } else {
        qDebug() << "Error:" << reply->errorString().toLatin1().data();
        QMessageBox::warning(this,"天气","请求数据失败",QMessageBox::Ok);
    }

    reply->deleteLater(); // 确保释放资源
}


