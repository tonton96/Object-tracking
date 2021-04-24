#include "objecttracking.h"
#include <QDebug>
using namespace cv;
using namespace std;

#define SSTR( x ) static_cast< std::ostringstream & >( \
    ( std::ostringstream() << std::dec << x ) ).str()

ObjectTracking::ObjectTracking(std::string path)
{
    isTracking= false;
    video.open(path);
    tracker = TrackerKCF::create();
}

float ObjectTracking::GetVideoFps(){
    if(video.isOpened()){
        return video.get(CAP_PROP_FPS);
    }
    return -1;
}

bool ObjectTracking::IsVideoOpened(){
    return video.isOpened();
}

ObjectTracking:: ~ObjectTracking(){
    video.release();
    cv::destroyAllWindows();
}

void ObjectTracking:: EnableTracking(){
    isTracking = true;
}

void ObjectTracking:: DisableTracking(){
    isTracking= false;
}

void ObjectTracking:: InitTracker(cv::Mat frame, cv::Rect trackingBox){
    this->trackingBox = trackingBox;
    tracker->init(frame, trackingBox);
}

bool ObjectTracking::IsTracking(){
    return isTracking;
}

cv::Mat ObjectTracking:: GetFrame(){
    Mat frame;
    video>>frame;

    if(isTracking && !frame.empty()){
        double timer = (double)getTickCount();
        if (tracker->update(frame, trackingBox)) {
            cv::rectangle(frame, trackingBox, cv::Scalar(255, 0, 0), 2, 8);
        }
        else{
            isTracking= false;
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
        putText(frame, "KCF Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
        putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
    }
    return frame;
}
