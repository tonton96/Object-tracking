#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/*
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/highgui.hpp>
#include "QDebug"
#include "objecttracking.h"

using namespace cv;
using namespace std;

#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

void Show();

int main()
{
    //Show();
    ObjectTracking obj("C:\\Users\\lcv06\\OneDrive\\Desktop\\Object tracking\\Video test\\test.mp4");
    //obj.SelectRoi();
    obj.PlayVideo();
    qDebug()<<"End";
    return 0;
}

void Show(){
    // Create video capturing object
    // 0 opens default camera, otherwise filename as argument
    cv::VideoCapture video("C:\\Users\\lcv06\\OneDrive\\Desktop\\Object tracking\\Video test\\test.mp4");

    // Check that video is opened
    if (!video.isOpened()) return;

    // For saving the frame
    cv::Mat frame;

    // Create tracker, select region-of-interest (ROI) and initialize the tracker
    cv::Ptr<cv::Tracker> tracker = cv::TrackerKCF::create();
    video.read(frame);
    cv::Rect trackingBox = cv::selectROI(frame, false);
    cv::destroyWindow("ROI selector");
    tracker->init(frame, trackingBox);

    // Loop through available frames
    while (video.read(frame)) {
        // Start timer
        double timer = (double)getTickCount();

        // Update the tracker and draw the rectangle around target if update was successful
        if (tracker->update(frame, trackingBox)) {
            cv::rectangle(frame, trackingBox, cv::Scalar(255, 0, 0), 2, 8);
        }
        else{
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
        // Display tracker type on frame
        putText(frame, "KCF Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
        // Display FPS on frame
        putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
        // Display the frame
        cv::imshow("Video feed", frame);

        // For breaking the loop
        if (cv::waitKey(1) >= 0) break;
    }

    video.release();
    cv::destroyAllWindows();
}
*/
