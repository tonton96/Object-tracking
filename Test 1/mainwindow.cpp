#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <QDebug>
#include <graphicsscene.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isPlaying= false;
    objTracking= nullptr;
    GraphicsScene *graphicsScene= new GraphicsScene(this);
    graphicsScene->SetMain(this);
    ui->graphicsView->setScene(graphicsScene);
    ui->graphicsView->scene()->addItem(&pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(objTracking!= nullptr){
        delete  objTracking;
        objTracking=nullptr;
    }
}

void MainWindow:: InitNewVideo(QString path){
    std::string stdpath = path.toStdString();
    objTracking = new ObjectTracking(stdpath);
    isPlaying= false;
    ui->btnPlay->setText("Play");
}

void MainWindow:: PlayVideo(){
    if(objTracking->IsVideoOpened()){
        isPlaying= true;
        ui->btnPlay->setText("Pause");
        while (objTracking->IsVideoOpened() && isPlaying) {
            cv::Mat frame = objTracking->GetFrame();
            if(!frame.empty())
            {
                QImage qimg(frame.data,
                            frame.cols,
                            frame.rows,
                            frame.step,
                            QImage::Format_RGB888);
                pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
                ui->graphicsView->fitInView(&pixmap, Qt::IgnoreAspectRatio);
            }
            else{
                delete objTracking;
                objTracking= nullptr;
                isPlaying = false;
                ui->btnPlay->setText("Play");
                break;
            }
            if(objTracking!=nullptr){
                if(!objTracking->IsTracking()){
                    int fps = objTracking->GetVideoFps();
                    cv::waitKey(1000/fps);
                }
                else{
                    cv::waitKey(1);
                }
            }
            qApp->processEvents();
        }
    }
}


void MainWindow:: SelectRoi(cv::Rect trackingBox){
    if(objTracking != nullptr){
        if(objTracking->IsVideoOpened()){
            cv::Mat frame = objTracking->GetFrame();
            if(!frame.empty()){
                if(trackingBox.width>0&&trackingBox.height){
                    objTracking->InitTracker(frame,trackingBox);
                    objTracking->EnableTracking();
                }
            }
        }
    }
}

void MainWindow:: SelectRoi(){
    if(objTracking != nullptr){
        if(objTracking->IsVideoOpened()){
            cv::Mat frame = objTracking->GetFrame();
            if(!frame.empty()){
                isPlaying= false;
                cv::Rect trackingBox = cv::selectROI("windownName",frame, false);
                cv::destroyAllWindows();
                isPlaying= true;
                if(trackingBox.width>0&&trackingBox.height){
                    objTracking->InitTracker(frame,trackingBox);
                    objTracking->EnableTracking();
                }
            }
        }
    }
}

void MainWindow::on_btnSelectFile_clicked()
{
    if(objTracking!=nullptr){
        delete (objTracking);
    }
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open file"),
                QCoreApplication::applicationDirPath(),
                "Video file(*.mp4);;Video file (*.avi)"
                );
    if(fileName.count()>0){
        ui->lblName->setText(fileName);
    }
    else{
        ui->lblName->setText("Error");
    }
}

void MainWindow::on_btnPlay_clicked()
{
    if(isPlaying){
        PauseVideo();
    }
    else{
        if(objTracking == nullptr){
            if(ui->lblName->text().length()>0){
                InitNewVideo(ui->lblName->text());
            }
        }
        if(objTracking!=nullptr){
            if(objTracking->IsVideoOpened()){
                PlayVideo();
            }
        }
    }
}

void MainWindow:: PauseVideo(){
    if(objTracking->IsVideoOpened()){
        isPlaying= false;
        ui->btnPlay->setText("Play");
    }
}

void MainWindow::on_btnSelect_clicked()
{
    if(objTracking!=nullptr){
        SelectRoi();
    }
}

void MainWindow::OnReleaseMouse(QGraphicsSceneMouseEvent * mouseEvent){
    if(objTracking!=nullptr){
        to = mouseEvent->scenePos();
        int imgWidth = objTracking->width;
        int imgHeight = objTracking->height;

        int xfrom = from.x();
        int yfrom = from.y();
        int xto = to.x();
        int yto = to.y();

        if(xfrom<0){
            xfrom=0;
        }
        else if(xfrom>=imgWidth){
            xfrom = imgWidth-1;
        }

        if(yfrom<0){
            yfrom=0;
        }
        else if(yfrom>=imgHeight){
            yfrom = imgHeight-1;
        }

        if(xto<0){
            xto=0;
        }
        else if(xto>=imgWidth){
            xto = imgWidth-1;
        }

        if(yto<0){
            yto=0;
        }
        else if(yto>=imgHeight){
            yto = imgHeight-1;
        }

        int xtop = xfrom<xto?xfrom:xto;
        int ytop = yfrom<yto?yfrom:yto;
        int w = xfrom > xto?(xfrom-xto):(xto-xfrom);
        int h= yfrom>yto?(yfrom-yto):(yto-yfrom);

        cv::Rect box(xtop, ytop,w,h);
        SelectRoi(box);
    }
}

void MainWindow::OnPressMouse(QGraphicsSceneMouseEvent * mouseEvent){
    if(objTracking!=nullptr){
        from = mouseEvent->scenePos();
    }
}

void MainWindow::OnMoveMouse(QGraphicsSceneMouseEvent * mouseEvent){
    if(objTracking!=nullptr){
        //to = mouseEvent->scenePos();
    }
}

void MainWindow::OnDoubleClickMouse(QGraphicsSceneMouseEvent *mouseEvent){

}

void MainWindow::closeEvent (QCloseEvent *event){
    if(objTracking!=nullptr){
        delete objTracking;
        objTracking= nullptr;
    }
}
