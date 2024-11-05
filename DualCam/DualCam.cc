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
    // Create SLAM system for stereo camera (using both cameras for stereo input)
    ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::STEREO, true);

    // Open the first camera (e.g., /dev/video5)
    cv::VideoCapture cap1("/dev/video5");
    if (!cap1.isOpened())
    {
        std::cerr << "Failed to open camera 1!" << std::endl;
        return 1;
    }

    // Open the second camera (e.g., /dev/video6)
    cv::VideoCapture cap2("/dev/video6");
    if (!cap2.isOpened())
    {
        std::cerr << "Failed to open camera 2!" << std::endl;
        return 1;
    }

    // Set camera properties (if necessary) for both cameras
    cap1.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap1.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap1.set(cv::CAP_PROP_FPS, 30);

    cap2.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap2.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap2.set(cv::CAP_PROP_FPS, 30);

    cv::Mat frame1, frame2;
    while (true)
    {
        // Capture frames from both cameras
        cap1 >> frame1;
        cap2 >> frame2;

        if (frame1.empty() || frame2.empty())
        {
            std::cerr << "Failed to capture frame!" << std::endl;
            break;
        }

        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;
        
        try {
            SLAM.TrackStereo(frame1, frame2, timestamp);
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        // Display the frames from both cameras (optional)
        cv::imshow("Camera 1", frame1);
        cv::imshow("Camera 2", frame2);

        if (cv::waitKey(1) == 27) // Press 'ESC' to exit
        {
            break;
        }
    }

    // Stop all threads
    SLAM.Shutdown();

    return 0;
}
