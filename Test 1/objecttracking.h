#ifndef OBJECTTRACKING_H
#define OBJECTTRACKING_H

#include <QString>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/highgui.hpp>

class ObjectTracking
{
public:
    bool created;
private:
    bool isTracking;
    bool isPlay;
    const std::string windownName = "Object tracking";
    cv::VideoCapture video;
    cv::Mat frame;
    cv::Ptr<cv::Tracker> tracker;
    cv::Rect trackingBox;

public:    
    ObjectTracking(std::string path);
    void PlayVideo();
    void PauseVideo();
    void SelectRoi();
public:
    ~ObjectTracking();
};

#endif // OBJECTTRACKING_H
