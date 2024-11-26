#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> // for GaussianBlur
#include "System.h"

int main(int argc, char **argv)
{
    ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::MONOCULAR, true);

    cv::VideoCapture cap("/dev/video5");
    if (!cap.isOpened())
    {
        std::cerr << "Failed to open camera!" << std::endl;
        return 1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 30);

    cv::Mat frame, processed_frame;
    while (true)
    {
        cap >> frame;
        if (frame.empty())
        {
            std::cerr << "Failed to capture frame!" << std::endl;
            break;
        }

        // Apply Gaussian Blur to reduce noise
        cv::GaussianBlur(frame, processed_frame, cv::Size(3, 3), 1.5);

        // Track with ORB-SLAM using the processed frame
        SLAM.TrackMonocular(processed_frame, cap.get(cv::CAP_PROP_POS_FRAMES));

        // Display the original frame with optional processing for visualization
        cv::imshow("Camera", processed_frame);
        if (cv::waitKey(1) == 27) // Press 'ESC' to exit
        {
            break;
        }
    }

    SLAM.Shutdown();
    return 0;
}

