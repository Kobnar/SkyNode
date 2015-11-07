// main.cpp
#include "main.h"

int main()
{
    std::cout << "Program started.\n";

    // Read configuration file
    INIReader ini_reader("settings.ini");

    if (ini_reader.ParseError() < 0)
    {
        std::cout << "Failed to read program settings. Exiting.";
        return 1;
    }

    // Configure default settings
    bool debug_mode = ini_reader.GetBoolean(
        "SkyNode", "debug_mode", true);
    bool tracking_mode = ini_reader.GetBoolean(
        "SkyNode", "tracking_mode", true);
    long threshold_sensitivity = ini_reader.GetInteger(
        "SkyNode", "threshold_sensitivity", 50);
    long threshold_blur = ini_reader.GetInteger(
        "SkyNode", "threshold_blur", 10);

    // Get camera
    cv::VideoCapture capture(0);
    if (!capture.isOpened())
    {
        std::cout << "Camera not found.\n";
        return -1;
    }

    // Declare values
    cv::Mat frame_0;
    cv::Mat frame_1;
    cv::Mat grey_frame_0;
    cv::Mat grey_frame_1;
    cv::Mat difference_frame;
    cv::Mat threshold_frame;

    cv::namedWindow("camera", 1);

    // Capture first frame
    capture.read(frame_0);
    cv::cvtColor(frame_0, grey_frame_0, cv::COLOR_BGR2GRAY);

    // Process video stream
    std::cout << "Starting stream.\n";
    while (true)
    {
        // Capture new frame
        capture.read(frame_1);
        cv::cvtColor(frame_1, grey_frame_1, cv::COLOR_BGR2GRAY);

        // Difference the frame
        cv::absdiff(grey_frame_0, grey_frame_1, difference_frame);

        // Threshold values
        cv::threshold(difference_frame, threshold_frame, threshold_sensitivity, 255, cv::THRESH_BINARY);

        // Blur and re-threshold
        cv::blur(threshold_frame, threshold_frame, cv::Size(10, 10));
        cv::threshold(threshold_frame, threshold_frame, threshold_sensitivity, 255, cv::THRESH_BINARY);

        // Show the frame
        cv::imshow("camera", threshold_frame);

        // Cycle frames
        frame_1.copyTo(frame_0);
        grey_frame_1.copyTo(grey_frame_0);

        // Listen for keystrokes
        switch (cv::waitKey(10))
        {
            case 27:    // 'esc' key has been pressed
                std::cout << "Ending program.\n";
                return 0;
            case 116:   // 't' has been pressed
                tracking_mode = !tracking_mode;
                if (!tracking_mode) std::cout << "Motion tracking disabled.\n";
                else std::cout << "Motion tracking enabled.\n";
                break;
            case 100:   // 'd' has been pressed
                debug_mode = !debug_mode;
                if (!debug_mode) std::cout << "Debug mode disabled.\n";
                else std::cout << "Debug mode enabled.\n";
                break;
            case 43:    // '+' has been pressed
                if (threshold_sensitivity < 100)
                {
                    threshold_sensitivity += 5;
                    std::cout << "Sensitivity: " << threshold_sensitivity <<"\n";
                }
                break;
            case 45:    // '-' has been pressed
                if (threshold_sensitivity > 0)
                {
                    threshold_sensitivity -= 5;
                    std::cout << "Sensitivity: " << threshold_sensitivity <<"\n";
                }
                break;
        }
    }

    return 0;
}