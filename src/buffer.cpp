#include "buffer.h"

using namespace std;
using namespace cv;
namespace skynode {
	
	// TODO: Fetch settings from file/args
	const Size Buffer::PREPROCESS_BLUR_ = Size(4, 4);
	const int Buffer::MIN_DIFF_THRESHOLD_ = 8;
	const Size Buffer::NOISE_ERODE_ = Size(2, 2);
	
	// Primary constructor.
	Buffer::Buffer(VideoCapture capture_source)
	{
		// Save the VideoSource object
		source_ = capture_source;
		
		// Capture and pre-process the initial matrix
		source_.read(raw_[cursor_]);
		preProcess(cursor_);
		
		// Store the video capture source dimensions
		matrix_size_ = raw_[cursor_].size();
		
		// Create an empty difference matrix
		difference_ = Mat::zeros(
			matrix_size_.height,
			matrix_size_.width,
			CV_8UC1);
		
	} // Buffer::Buffer(VideoCapture capture_source)
	
	// Pre-processes a raw image matrix.
	void Buffer::preProcess(int idx)
	{
		// Blur the matrix
		blur(
			raw_[idx],
			processed_[idx],
			PREPROCESS_BLUR_);
		
		// Convert the matrix to grayscale
		cvtColor(
			processed_[idx],
			processed_[idx],
			COLOR_BGR2GRAY);
	} // void Buffer::preProcess(int idx)
	
	// Generates the absolute difference matrix
	void Buffer::genDifference(int idx)
	{
		// Calculate the absolute difference between the matrix at cursor_ and idx
		absdiff(
			processed_[cursor_],
			processed_[idx],
			difference_);
			
		// Calculate the mean intensity of values greater than MIN_DIFF_THRESHOLD_
		int mean_intensity = ceil(mean(difference_, difference_ > MIN_DIFF_THRESHOLD_)[0]);
		
		// If any changes are detected, process a new difference map
		if (mean_intensity > 1)
		{
			// Create a binary threshold image based on the mean intensity value
			threshold(
				difference_,
				difference_,
				mean_intensity, 255, THRESH_BINARY);
				
			// Erode blobs to further mitigate noise
			erode(
				difference_,
				difference_,
				getStructuringElement(MORPH_RECT, NOISE_ERODE_));
				
			// Dialte blobs to connect bits
			dilate(
				difference_,
				difference_,
				getStructuringElement(MORPH_RECT, NOISE_ERODE_));
		}
		else // Otherwise create an empty difference map
		{
		
			difference_ = Mat::zeros(
				matrix_size_.height,
				matrix_size_.width,
				CV_8UC1);
		}
	}
	
	void Buffer::detectMovement()
	{
        // Declare vectors for contour data
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hirearchy;

        // Find contours
        cv::findContours(
            difference_,
            contours, hirearchy,
            CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

        // Calculate moments (i.e. blobs)
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
		
		getRaw().copyTo(debug_);

		// Declare rectangles
		std::vector<std::vector<cv::Point>> contours_poly( contours.size() );
		std::vector<cv::Rect> bound_rect( contours.size() );
		for (int i = 0; i < contours.size(); i++)
		{
			cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true);
			bound_rect[i] = cv::boundingRect( cv::Mat(contours_poly[i]) );
		}

		// Draw contour lines
		for (int i = 0; i < contours.size(); i++)
		{
			cv::Scalar color = cv::Scalar(255, 255, 255);
			cv::drawContours( debug_, contours, i, color, 1, 8, hirearchy, 0, cv::Point());
			cv::rectangle( debug_, bound_rect[i].tl(), bound_rect[i].br(), color, 1, 8, 0);
			cv::circle( debug_, mc[i], 4, color, -1, 8, 0 );
		}
	}
	
	// Cycle the active frames
	void Buffer::step()
	{
		// Calculate the new matrix' index
		int new_cursor = (cursor_ + 1) % BUFFER_LENGTH_;
		
		// Capture and pre-process a new image matrix
		source_.read(raw_[new_cursor]);
		preProcess(new_cursor);
		
		// Generate the absolute difference matrix
		genDifference(new_cursor);
		
		// Detect movement
		detectMovement();
		
		// Update the buffer index
		cursor_ = new_cursor;
	} // void Buffer::step()
	
	// Returns the most recent raw image matrix.
	Mat Buffer::getRaw()
	{
		return raw_[cursor_];
	} // Mat Buffer::getRaw()
	
	// Returns the most recent pre-processed image matrix.
	Mat Buffer::getProcessed()
	{
		return processed_[cursor_];
	} // Mat Buffer::getProcessed()
	
	// Returns the most recent absolute difference matrix.
	Mat Buffer::getDifference()
	{
		return difference_;
	} // Mat Buffer::getDifference()
	
	// Returns the most recent absolute difference matrix.
	Mat Buffer::getDebug()
	{
		return debug_;
	} // Mat Buffer::getDebug()
}