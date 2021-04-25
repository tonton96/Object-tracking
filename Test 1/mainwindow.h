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
public:
    void InitNewVideo(QString path);
    void PlayVideo();
    void PauseVideo();
    void SelectRoi(cv::Rect rect);
    void SelectRoi();
    void OnReleaseMouse(QGraphicsSceneMouseEvent * mouseEvent);
    void OnPressMouse(QGraphicsSceneMouseEvent * mouseEvent);
    void OnMoveMouse(QGraphicsSceneMouseEvent * mouseEvent);
    void OnDoubleClickMouse(QGraphicsSceneMouseEvent * mouseEvent);

private slots:
    void on_btnSelectFile_clicked();
    void on_btnPlay_clicked();
    void on_btnSelect_clicked();
    void closeEvent (QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    ObjectTracking *objTracking;
    bool isPlaying;
    QGraphicsPixmapItem pixmap;

    QPointF from, to;
};
#endif // MAINWINDOW_H
