#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>

#include <objecttracking.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void InitNewVideo(QString path);
    void PlayVideo();
    void PauseVideo();
    void SelectRoi(cv::Rect rect);
    cv::Rect GetRectBy2Point(QPointF from, QPointF to, int width, int height);
public:
    void OnReleaseMouse(QGraphicsSceneMouseEvent * mouseEvent);
    void OnPressMouse(QGraphicsSceneMouseEvent * mouseEvent);
    void OnMoveMouse(QGraphicsSceneMouseEvent * mouseEvent);

private slots:
    void on_btnSelectFile_clicked();
    void on_btnPlay_clicked();
    void closeEvent (QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    ObjectTracking *objTracking;
    bool isPlaying;
    bool isDraging;
    QGraphicsPixmapItem pixmap;
    QGraphicsRectItem* rectangle;
    QPointF from, to;
};
#endif // MAINWINDOW_H
