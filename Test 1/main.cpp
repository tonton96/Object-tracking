
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
    Show();
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
/*
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

using namespace cv;
using namespace std;

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

int main(int argc, char **argv)
{
    // List of tracker types in OpenCV 3.4.1
    string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};
    // vector <string> trackerTypes(types, std::end(types));

    // Create a tracker
    string trackerType = trackerTypes[2];

    Ptr<Tracker> tracker;

    #if (CV_MINOR_VERSION < 3)
    {
        tracker = Tracker::create(trackerType);
    }
    #else
    {
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
        if (trackerType == "CSRT")
            tracker = TrackerCSRT::create();
    }
    #endif
    // Read video
    VideoCapture video("C:\\Users\\lcv06\\OneDrive\\Desktop\\Object tracking\\Video test\\test.mp4");

    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }

    // Read first frame
    Mat frame;
    bool ok = video.read(frame);

    // Define initial bounding box
    Rect bbox(287, 23, 86, 320);

    // Uncomment the line below to select a different bounding box
     bbox = selectROI(frame, false);
     //Display bounding box.
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );

    imshow("Tracking", frame);
    tracker->init(frame, bbox);

    while(video.read(frame))
    {
        // Start timer
        double timer = (double)getTickCount();

        // Update the tracking result
        bool ok = tracker->update(frame, bbox);

        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }

        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);

        // Display FPS on frame
        putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);

        // Display frame.
        imshow("Tracking", frame);

        // Exit if ESC pressed.
        int k = waitKey(1);
        if(k == 27)
        {
            break;
        }

    }
}
*/
