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
    created=video.isOpened();
}

ObjectTracking:: ~ObjectTracking(){
    video.release();
    cv::destroyAllWindows();
}

void ObjectTracking::PlayVideo(){
    isPlay= true;
    while(isPlay && video.read(frame)){
        namedWindow(windownName, WINDOW_GUI_NORMAL);
        resizeWindow(windownName, 1280,720);

        if(isTracking){
            double timer = (double)getTickCount();
            if (tracker->update(frame, trackingBox)) {
                cv::rectangle(frame, trackingBox, cv::Scalar(255, 0, 0), 2, 8);
            }
            else{
                putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
            }
            float fps = getTickFrequency() / ((double)getTickCount() - timer);
            putText(frame, "KCF Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
            putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
            if(isPlay){
                cv::imshow(windownName, frame);
                cv::waitKey(1);
            }
        }
        else{
            if(isPlay){
                cv::imshow(windownName, frame);
                cv::waitKey(30);
            }
        }
    }
    if(isPlay){
        video.release();
    }
}

void ObjectTracking::PauseVideo(){
    isPlay= false;
}

void ObjectTracking::SelectRoi(){
    if(frame.cols>0&&frame.rows>0){
        isPlay= false;
        namedWindow(windownName, WINDOW_GUI_NORMAL);
        resizeWindow(windownName, 1280,720);
        Rect trackingBox = cv::selectROI(windownName,frame, false);
        tracker->init(frame, trackingBox);
        isTracking= true;
        PlayVideo();
    }
}
