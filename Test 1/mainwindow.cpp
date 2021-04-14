#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isPlaying= false;
    objTracking= nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete  objTracking;
}

void MainWindow:: InitNewVideo(std::string path){
    objTracking = new ObjectTracking(path);
    if(objTracking->created){
        ui->lblName->setText(QString::fromUtf8(path.c_str()));
        isPlaying= false;
        ui->btnPlay->setEnabled(true);
        ui->btnSelect->setEnabled(true);
    }
    else{
        ui->btnPlay->setEnabled(false);
        ui->btnSelect->setEnabled(false);
    }
}

void MainWindow:: PlayVideo(){
    isPlaying= true;
    ui->btnPlay->setText("Pause");
    objTracking->PlayVideo();
}

void MainWindow:: PauseVideo(){
    isPlaying= false;
    ui->btnPlay->setText("Play");
    objTracking->PauseVideo();
}

void MainWindow:: SelectRoi(){
    objTracking->SelectRoi();
}

void MainWindow::on_btnSelectFile_clicked()
{
    if(objTracking!=nullptr){
        delete (objTracking);
    }

    /*
     * get video path
    */

    InitNewVideo("C:\\Users\\lcv06\\OneDrive\\Desktop\\Object tracking\\Video test\\test.mp4");
}

void MainWindow::on_btnPlay_clicked()
{
    if(isPlaying){
        PauseVideo();
    }
    else{
        if(objTracking!=nullptr){
            PlayVideo();
        }
    }
}

void MainWindow::on_btnSelect_clicked()
{
    if(objTracking!=nullptr){
        SelectRoi();
    }
}

void MainWindow::closeEvent (QCloseEvent *event){
    delete objTracking;
    objTracking= nullptr;
}
