#ifndef OBJECTTRACKING_H
#define OBJECTTRACKING_H

#include <QString>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/highgui.hpp>

class ObjectTracking
{
private:
    bool isTracking;
    cv::VideoCapture video;
    cv::Ptr<cv::Tracker> tracker;
    cv::Rect trackingBox;    
public:
    int width;
    int height;
public:    
    ObjectTracking(std::string path);
    float GetVideoFps();
    bool IsVideoOpened();
    bool IsTracking();
    void EnableTracking();
    void DisableTracking();
    void InitTracker(cv::Mat frame, cv::Rect trackingBox);
    cv::Mat GetFrame();
public:
    ~ObjectTracking();
};

#endif // OBJECTTRACKING_H
