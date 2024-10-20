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
    // ORB-SLAM3システムの作成（設定ファイルと辞書ファイルを読み込み）
    ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::MONOCULAR, true);

    // iPhone 16 カメラのオープン (仮想デバイスの場合 /dev/video5 を仮定)
    cv::VideoCapture cap("/dev/video5");
    if (!cap.isOpened())
    {
        std::cerr << "Failed to open camera!" << std::endl;
        return 1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 854);  
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 30);            // フレームレート設定

    // ウィンドウ表示の準備
    cv::Mat frame;
    while (true)
    {
        // フレームを取得
        cap >> frame;
        if (frame.empty())
        {
            std::cerr << "Failed to capture frame!" << std::endl;
            break;
        }

        // 現在のフレームをSLAMシステムに渡してトラッキング
        SLAM.TrackMonocular(frame, cap.get(cv::CAP_PROP_POS_FRAMES));

        // フレームの表示（任意）
        cv::imshow("Camera", frame);
        if (cv::waitKey(1) == 27) // 'ESC'キーで終了
        {
            break;
        }
    }

    // マップデータの保存 (保存先ディレクトリが存在することを確認)
    if(!SLAM.SaveMap("./saved_map/saved_map.bin"))
    {
        std::cerr << "Failed to save the map." << std::endl;
    }

    // すべてのスレッドの停止
    SLAM.Shutdown();

    return 0;
}

