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
    isDraging= false;
    objTracking= nullptr;
    rectangle=nullptr;
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
        while (objTracking!=nullptr) {
            if(!(objTracking->IsVideoOpened() && isPlaying)){
                break;
            }
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
                if(rectangle!= nullptr){
                    isDraging = false;
                    ui->graphicsView->scene()->removeItem(rectangle);
                    delete  rectangle;
                    rectangle = nullptr;
                }
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

void MainWindow::on_btnSelectFile_clicked()
{
    PauseVideo();
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open file"),
                QCoreApplication::applicationDirPath(),
                "Video file(*.mp4);;Video file (*.avi)"
                );
    if(fileName.count()>0){
        if(objTracking!=nullptr){
            delete objTracking;
            objTracking = nullptr;
        }
        ui->lblName->setText(fileName);
        InitNewVideo(fileName);
    }
    else{
        if(objTracking!=nullptr){
            if(objTracking->IsVideoOpened()){
                PlayVideo();
            }
        }
    }
}

void MainWindow::on_btnPlay_clicked()
{
    if(isPlaying){
        PauseVideo();
    }
    else{        
        if(rectangle!=nullptr){
            delete  rectangle;
            rectangle =nullptr;
        }
        if(objTracking!=nullptr){
            if(objTracking->IsVideoOpened()){
                PlayVideo();
            }
        }
    }
}

void MainWindow:: PauseVideo(){
    isPlaying= false;
    ui->btnPlay->setText("Play");
    if(rectangle!=nullptr){
        delete  rectangle;
        rectangle =nullptr;
    }
}

void MainWindow::OnReleaseMouse(QGraphicsSceneMouseEvent * mouseEvent){
    if(objTracking!=nullptr){
        to = mouseEvent->scenePos();
        int imgWidth = objTracking->width;
        int imgHeight = objTracking->height;
        cv::Rect box = GetRectBy2Point(from, to, imgWidth, imgHeight);
        SelectRoi(box);
        if(isPlaying){
            delete  rectangle;
            rectangle =nullptr;
        }
        isDraging= false;
    }
}

void MainWindow::OnPressMouse(QGraphicsSceneMouseEvent * mouseEvent){
    if(objTracking!=nullptr){
        from = mouseEvent->scenePos();
        isDraging= true;
    }
}

void MainWindow::OnMoveMouse(QGraphicsSceneMouseEvent * mouseEvent){
    if(objTracking!=nullptr && isDraging){
        to = mouseEvent->scenePos();
        int imgWidth = objTracking->width;
        int imgHeight = objTracking->height;
        cv::Rect box = GetRectBy2Point(from, to, imgWidth, imgHeight);
        if(rectangle!= nullptr){
            ui->graphicsView->scene()->removeItem(rectangle);
            delete  rectangle;
            rectangle = nullptr;
        }
        rectangle = new QGraphicsRectItem(box.x,box.y,box.width,box.height);
        rectangle->setPen(QPen(Qt::red));
        ui->graphicsView->scene()->addItem(rectangle);
    }
}

void MainWindow::closeEvent (QCloseEvent *event){
    isPlaying= false;
}

cv::Rect MainWindow:: GetRectBy2Point(QPointF from, QPointF to, int width, int height){
    int imgWidth = width;
    int imgHeight = height;

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

    return cv::Rect(xtop, ytop,w,h);
}
