// main.h
#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <vector>

#include "lib/inih/ini.c"
#include "lib/inih/cpp/INIReader.h"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

namespace SkyNode
{
    // Tracks the program settings
    bool DEBUG_MODE;
    bool TRACKING_MODE;
    long RAW_THRESHOLD;
    long BLUR_THRESHOLD;
    cv::Size ERODE;
    cv::Size BLUR;

    // Loads SkyNode settings
    void loadSettings();

    struct Track
    {
        int x, y, width, height;
    };

    // An individual frame which has been captured and processed for tracking.
    class Frame
    {
    private:
        cv::Mat raw_, processed_;

    public:
        // Default constructor
        Frame() {};

        // Captures a new frame from an existing video source
        Frame(cv::VideoCapture& capture_source);

        // Copies an existing frame
        Frame(Frame& frame);

        cv::Mat getRaw();
        cv::Mat getProcessed();
    };

    class Tracker
    {
    private:
        cv::VideoCapture capture_source_;
        Frame frame_0_, frame_1_;
        cv::Mat difference_;

        // Generates a matrix of differences between frame_0 and frame_1
        void genDifference();

    public:
        // Default constructor
        Tracker() {};

        // Captures a new frame from an existing video source
        Tracker(cv::VideoCapture& capture_source);

        // Cycles frame_0 and frame_1, generates a new difference
        void update();

        // Returns the newest frame
        cv::Mat getRaw();

        // Returns the difference matrix between frame_0 and frame_1
        cv::Mat getDifference();

    };

    // Compiles a matrix of differences between two frames
    void compareFrames(Frame& frame_0, Frame& frame_1, cv::Mat& mat_diff);

    // Detects contours
    void getContours(Frame& frame_0, Frame& frame_1, cv::Mat mat_diff);
}

int main();

#endif // MAIN_H_