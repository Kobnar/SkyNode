// main.cpp
#include "main.h"

int main()
{
    std::cout << "Program started.\n";

    // Read configuration file and set default states

    INIReader ini_reader("settings.ini");

    if (ini_reader.ParseError() < 0)
    {
        std::cout << "Failed to read program settings. Exiting.";
        return 1;
    }

    SkyNode::DEBUG_MODE = ini_reader.GetBoolean(
        "SkyNode", "debug_mode", true);

    SkyNode::TRACKING_MODE = ini_reader.GetBoolean(
        "SkyNode", "tracking_mode", true);

    SkyNode::FRAME_BLUR = ini_reader.GetInteger(
        "SkyNode", "frame_blur", 3);

    SkyNode::THRESHOLD_SENSITIVITY = ini_reader.GetInteger(
        "SkyNode", "threshold_sensitivity", 50);

    SkyNode::THRESHOLD_ERODE = ini_reader.GetInteger(
        "SkyNode", "threshold_erode", 3);

    SkyNode::THRESHOLD_DIALATE = ini_reader.GetInteger(
        "SkyNode", "threshold_dialate", 6);

    // Get camera

    cv::VideoCapture capture(0);

    if (!capture.isOpened())
    {
        std::cout << "Camera not found.\n";
        return -1;
    }

    // Declare target frame matrices

    cv::Mat frame_0, frame_1;
    cv::Mat blur_frame_0, blur_frame_1;
    cv::Mat grey_frame_0, grey_frame_1;
    cv::Mat track_frame;

    // Define target windows

    cv::namedWindow("camera", 1);
    cv::namedWindow("debug", 1);

    // Capture first frame

    capture.read(frame_0);
    cv::blur(frame_0, blur_frame_0, cv::Size(SkyNode::FRAME_BLUR, SkyNode::FRAME_BLUR));
    cv::cvtColor(blur_frame_0, grey_frame_0, cv::COLOR_BGR2GRAY);

    // Process video stream

    std::cout << "Starting stream.\n";
    while (true)
    {
        // Capture new frame
        capture.read(frame_1);
        cv::blur(frame_1, blur_frame_1, cv::Size(SkyNode::FRAME_BLUR, SkyNode::FRAME_BLUR));
        cv::cvtColor(blur_frame_1, grey_frame_1, cv::COLOR_BGR2GRAY);

        // Difference the frame
        cv::absdiff(grey_frame_0, grey_frame_1, track_frame);

        // Threshold values
        cv::threshold(track_frame, track_frame,
            SkyNode::THRESHOLD_SENSITIVITY, 255, cv::THRESH_BINARY);

        // Clean up noise
        cv::Mat erode_element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(SkyNode::THRESHOLD_ERODE, SkyNode::THRESHOLD_ERODE));
        cv::Mat dilate_element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(SkyNode::THRESHOLD_DIALATE, SkyNode::THRESHOLD_DIALATE));
        cv::erode(track_frame, track_frame, erode_element);
        cv::dilate(track_frame, track_frame, dilate_element);

        // Blur and re-threshold
        // cv::blur(threshold_frame, threshold_frame, cv::Size(10, 10));
        // cv::threshold(threshold_frame, threshold_frame, threshold_sensitivity, 255, cv::THRESH_BINARY);

        // Draw the source window
        cv::imshow("camera", frame_1);

        // Draw the debug window
        if (SkyNode::DEBUG_MODE)
        {
            cv::imshow("debug", track_frame);
        }
        else
        {
            cv::destroyWindow("debug");
        }

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
                SkyNode::TRACKING_MODE = !SkyNode::TRACKING_MODE;
                if (!SkyNode::TRACKING_MODE) std::cout << "Motion tracking disabled.\n";
                else std::cout << "Motion tracking enabled.\n";
                break;
            case 100:   // 'd' has been pressed
                SkyNode::DEBUG_MODE = !SkyNode::DEBUG_MODE;
                if (!SkyNode::DEBUG_MODE) std::cout << "Debug mode disabled.\n";
                else std::cout << "Debug mode enabled.\n";
                break;
            case 43:    // '+' has been pressed
                if (SkyNode::THRESHOLD_SENSITIVITY < 100)
                {
                    SkyNode::THRESHOLD_SENSITIVITY += 5;
                    std::cout << "Sensitivity: " << SkyNode::THRESHOLD_SENSITIVITY <<"\n";
                }
                break;
            case 45:    // '-' has been pressed
                if (SkyNode::THRESHOLD_SENSITIVITY > 0)
                {
                    SkyNode::THRESHOLD_SENSITIVITY -= 5;
                    std::cout << "Sensitivity: " << SkyNode::THRESHOLD_SENSITIVITY <<"\n";
                }
                break;
        }
    }

    return 0;
}