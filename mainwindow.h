#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMouseEvent>

#include <QNetworkAccessManager>
#include <QNetworkReply>

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

    //获取天气数据
    void getWeatherInfo(QString cityCode);

private:
    void onReplied(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QMenu *mExitMenu; // 右键退出的菜单
    QAction *mExitAct; // 退出的行为 - 菜单项

    bool mDragging; //
    QPoint mOffset;//窗口移动时，鼠标与左上角的偏移

    QNetworkAccessManager* mNetAccessManager;
};
#endif // MAINWINDOW_H
