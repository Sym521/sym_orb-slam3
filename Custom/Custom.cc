#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "System.h"

int main(int argc, char **argv)
{

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::MONOCULAR, true);

    // Open the default camera (usually the built-in camera)
    cv::VideoCapture cap("/dev/video5");
    if (!cap.isOpened())
    {
        std::cerr << "Failed to open camera!" << std::endl;
        return 1;
    }

    // Set camera properties (if necessary)
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 30);

    cv::Mat frame;
    while (true)
    {
        cap >> frame;
        if (frame.empty())
        {
            std::cerr << "Failed to capture frame!" << std::endl;
            break;
        }

        SLAM.TrackMonocular(frame, cap.get(cv::CAP_PROP_POS_FRAMES));

        // Display the frame (optional)
        cv::imshow("Camera", frame);
        if (cv::waitKey(1) == 27) // Press 'ESC' to exit
        {
            break;
        }
    }

    // Stop all threads
    SLAM.Shutdown();

    return 0;
}

