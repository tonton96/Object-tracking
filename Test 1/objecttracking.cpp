#include "objecttracking.h"
#include <QDebug>
using namespace cv;
using namespace std;

#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

ObjectTracking::ObjectTracking(std::string path)
{
    isTracking= false;
    video = VideoCapture(path);
    tracker = TrackerKCF::create();
}

ObjectTracking:: ~ObjectTracking(){
    qDebug()<<"Delete";
    video.release();
    cv::destroyAllWindows();
}

void ObjectTracking::PlayVideo(){
    isPlay=true;
    while(video.read(frame)&&isPlay){
        namedWindow("Video feed", WINDOW_GUI_NORMAL);
        resizeWindow("Video feed", 1280,720);
        double timer = (double)getTickCount();

        if(isTracking){
        // Update the tracker and draw the rectangle around target if update was successful
        if (tracker->update(frame, trackingBox)) {
            cv::rectangle(frame, trackingBox, cv::Scalar(255, 0, 0), 2, 8);
        }
        else{
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
        putText(frame, "KCF Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
        putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
        cv::imshow("Video feed", frame);
        cv::waitKey(1);
        }
        else{
            cv::imshow("Video feed", frame);
            cv::waitKey(30);
        }
    }
    if(isPlay){
        video.release();
        cv::destroyAllWindows();
    }
}

void ObjectTracking::PauseVideo(){
    isPlay= false;
}

void ObjectTracking::SelectRoi(){
    Rect trackingBox = cv::selectROI(frame, false);
    destroyWindow("ROI selector");
    tracker->init(frame, trackingBox);
}
