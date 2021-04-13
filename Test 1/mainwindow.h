#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <objecttracking.h>
#include <QCloseEvent>

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
    void InitNewVideo(std::string path);
    void PlayVideo();
    void PauseVideo();
    void SelectRoi();


private slots:
    void on_btnSelectFile_clicked();
    void on_btnPlay_clicked();
    void closeEvent (QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    ObjectTracking *objTracking;
    bool isPlaying;
};
#endif // MAINWINDOW_H
