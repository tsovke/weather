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


