// main.cpp
#include "main.h"

namespace SkyNode
{
    void loadSettings()
    {
        INIReader reader("settings.ini");

        std::cout << "Reading 'settings.ini':\n";

        DEBUG_MODE = reader.GetBoolean("SkyNode", "debug", true);
        std::cout << "\tDebug mode:\t" << DEBUG_MODE << "\n";

        TRACKING_MODE = reader.GetBoolean("SkyNode", "tracking", true);
        std::cout << "\tTracking mode:\t" << TRACKING_MODE << "\n";

        RAW_THRESHOLD = reader.GetInteger("SkyNode", "raw_threshold", 20);
        std::cout << "\tRaw threshold setting:\t" << RAW_THRESHOLD << "\n";

        BLUR_THRESHOLD = reader.GetInteger("SkyNode", "blur_threshold", 20);
        std::cout << "\tBlur threshold setting:\t" << BLUR_THRESHOLD << "\n";

        int erode_val = reader.GetInteger("SkyNode", "erode", 2);
        ERODE = cv::Size(erode_val, erode_val);
        std::cout << "\tErode setting:\t" << erode_val << " px\n";

        int blur_val = reader.GetInteger("SkyNode", "blur", 3);
        BLUR = cv::Size(blur_val, blur_val);
        std::cout << "\tBlur setting:\t" << blur_val << " px\n";

        if (reader.ParseError() < 0) throw -1;
    }

    Frame::Frame(cv::VideoCapture& capture_source)
    {
        // Capture new frame
        capture_source.read(raw_);

        // Blur and convert to B&W
        cv::blur(raw_, processed_, SkyNode::BLUR);
        cv::cvtColor(processed_, processed_, cv::COLOR_BGR2GRAY);
    }

    Frame::Frame(Frame& frame)
    {
        frame.getRaw().copyTo(raw_);
        frame.getProcessed().copyTo(processed_);
    }

    cv::Mat Frame::getRaw()
    {
        return raw_;
    }

    cv::Mat Frame::getProcessed()
    {
        return processed_;
    }

    Tracker::Tracker(cv::VideoCapture& capture_source)
    {
        capture_source_ = capture_source;
        frame_0_ = Frame(capture_source_);
        frame_1_ = frame_0_;
    }

    void Tracker::genDifference()
    {
        // Generate an absolute difference matrix between frame_0 and frame_1
        cv::absdiff(
            frame_0_.getProcessed(),
            frame_1_.getProcessed(),
            difference_);

        // Calculate a threshold matrix
        cv::threshold(
            difference_,
            difference_,
            SkyNode::RAW_THRESHOLD, 255, cv::THRESH_BINARY);

        // Erode blobs to remove noise
        cv::erode(
            difference_,
            difference_,
            cv::getStructuringElement(cv::MORPH_RECT, SkyNode::ERODE));

        // Blur blobs to make them bigger
        cv::blur(
            difference_,
            difference_,
            SkyNode::BLUR);

        // Calculate a new threshold matrix
        cv::threshold(
            difference_,
            difference_,
            BLUR_THRESHOLD, 255, cv::THRESH_BINARY);

        // Declare vectors for contour data
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hirearchy;

        // Find contours
        cv::findContours(
            difference_,
            contours, hirearchy,
            CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

        // Calculate moments (i.e. center of blobs)
        std::vector<cv::Moments> mu(contours.size());
        for (int i = 0; i < contours.size(); i++)
        {
            mu[i] = cv::moments(contours[i], false);
        }

        // Calculate centers of mass
        std::vector<cv::Point2f> mc(contours.size());
        for (int i = 0; i < contours.size(); i++)
        {
            mc[i] = cv::Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
        }

        // Draw difference matrix
        if (SkyNode::DEBUG_MODE)
        {
            frame_1_.getRaw().copyTo(difference_);
        }
        else
        {
            difference_ = cv::Mat::zeros(difference_.size(), CV_8UC3);
        }

        // Draw contour lines
        for (int i = 0; i < contours.size(); i++)
        {
            cv::Scalar color = cv::Scalar(255, 255, 255);
            cv::drawContours( difference_, contours, i, color, 1, 8, hirearchy, 0, cv::Point());
            // cv::circle( difference_, mc[i], 4, color, -1, 8, 0 );
        }
    }

    void Tracker::update()
    {
        // Cycle old frame assigment
        frame_0_ = Frame(frame_1_);

        // Capture new frame
        frame_1_ = Frame(capture_source_);

        // Generate a new difference matrix between frame_0 and frame_1
        genDifference();
    }

    cv::Mat Tracker::getRaw()
    {
        return frame_1_.getRaw();
    }

    cv::Mat Tracker::getDifference()
    {
        return difference_;
    }
}

int main()
{
    std::cout << "Program started.\n";

    // Load configuration settings
    try
    {
        SkyNode::loadSettings();
    }
    catch (int err)
    {
        std::cout << "Failed to read program settings. Exiting.";
        return -1;
    }

    // Get camera
    cv::VideoCapture capture_source(0);
    if (!capture_source.isOpened())
    {
        std::cout << "Failed to load camera. Exiting.\n";
        return -1;
    }

    // Define target windows
    cv::namedWindow("camera", 1);

    // Declare tracker
    SkyNode::Tracker tracker(capture_source);

    // Process video stream
    std::cout << "Starting stream.\n";
    while (true)
    {
        // Update the tracker with a new frame
        tracker.update();

        // Draw the source window
        cv::imshow("camera", tracker.getRaw());

        // Draw the debug window
        if (SkyNode::DEBUG_MODE)
        {
            cv::imshow("debug", tracker.getDifference());
        }
        else
        {
            cv::destroyWindow("debug");
        }

        // Listen for keystrokes
        switch (cv::waitKey(50))
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
        }
    }

    return 0;
}