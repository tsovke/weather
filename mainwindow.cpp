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
    QUrl url("https://www.ithome.com/");
qDebug()<<cityCode;
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
     mToday.ganmao=objData.value("ganmao").toString();
     mToday.wendu=objData.value("wendu").toInt();
     mToday.shidu=objData.value("shidu").toString();
     mToday.pm25=objData.value("pm25").toInt();
     mToday.quality=objData.value("quality").toString();
     //5. 今天的其它数据在mDay[1]中
     mToday.type=mDay[1].type;
     mToday.fl=mDay[1].fl;
     mToday.fx=mDay[1].fx;
     mToday.high=mDay[1].high;
     mToday.low=mDay[1].low;

    //6. 更新UI
     updateUI();
}

void MainWindow::updateUI() {
    //1. 更新日期和城市
    ui->lblDate->setText(QDateTime::fromString(mToday.date, "yyyyMMdd").toString("yyyy/MM/dd") + " "
                         + mDay[1].week);
    ui->lblCity->setText(mToday.city);

    //2. 更新今天数据
    ui->lblTemp->setText(QString::number(mToday.wendu));
    ui->lblType->setText(mToday.type);
    ui->lblLowHigh->setText(QString::number(mToday.low)+"~"+QString::number(mToday.high)+"℃");

    ui->lblGanmao->setText(mToday.ganmao);
    ui->lblWindFx->setText(mToday.fx);
    ui->lblWindFl->setText(mToday.fl);

    ui->lblPM25->setText(QString::number(mToday.pm25));
    ui->lblShidu->setText(mToday.shidu);
    ui->lblQuality->setText(mToday.quality);

    //3. 更新6天数据
    for (int i = 0; i < 6; ++i) {
        //3.1 更新日期和时间
        mWeekList[i]->setText("周"+mDay[i].week.right(1));
        QStringList ymdList=mDay[i].date.split("-");
        mDateList[i]->setText(ymdList[1]+"/"+ymdList[2]);

        //3.2 更新天气类型
        mTypeList[i]->setText(mDay[i].type);

        //3.3 更新空气质量
        if (mDay[i].aqi >= 0 && mDay[i].aqi <= 50) {
            mAqiList[i]->setText("优");
            mAqiList[i]->setStyleSheet("background-color: rgb(121,184,0);");
        } else if (mDay[i].aqi >50 && mDay[i].aqi <= 100) {
            mAqiList[i]->setText("良");
            mAqiList[i]->setStyleSheet("background-color: rgb(255,187,23);");
        } else if (mDay[i].aqi > 100 && mDay[i].aqi <= 150) {
            mAqiList[i]->setText("轻度");
            mAqiList[i]->setStyleSheet("background-color: rgb(255,87,97);");
        } else if (mDay[i].aqi >150 && mDay[i].aqi <= 200) {
            mAqiList[i]->setText("中度");
            mAqiList[i]->setStyleSheet("background-color: rgb(235,17,27);");
        } else if (mDay[i].aqi > 200 && mDay[i].aqi <= 250) {
            mAqiList[i]->setText("重度");
            mAqiList[i]->setStyleSheet("background-color: rgb(170,0,0);");
        }  else {
            mAqiList[i]->setText("严重");
            mAqiList[i]->setStyleSheet("background-color: rgb(110,0,0);");
        }
    }
    // 日期习惯
    ui->lblWeek0->setText("昨天");
    ui->lblWeek1->setText("今天");
    ui->lblWeek2->setText("明天");



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


