#include "buffer.h"

using namespace std;
using namespace cv;
namespace skynode {
	
	Buffer::Buffer(VideoCapture capture_source)
	{
		// Save the VideoSource object
		source_ = capture_source;
		
		// Capture and pre-process the initial matrix
		preProcess(cursor_);
		
		// Create an empty difference matrix
		difference_ = Mat::zeros(
			raw_[cursor_].rows,
			raw_[cursor_].cols,
			CV_8UC1);
		
	} // Buffer::Buffer(VideoCapture capture_source)
	
	void Buffer::preProcess(int idx)
	{
		// Capture a new matrix
		source_.read(raw_[idx]);
		
		// Blur the new matrix
		blur(
			raw_[idx],
			processed_[idx],
			Size(PREPROCESS_BLUR_, PREPROCESS_BLUR_));
		
		// Convert the new matrix to grayscale
		cvtColor(
			processed_[idx],
			processed_[idx],
			COLOR_BGR2GRAY);
	} // void Buffer::preProcess(int idx)
	
	void Buffer::step()
	{
		// Calculate the new matrix' index
		int new_cursor = (cursor_ + 1) % BUFFER_LENGTH_;
		
		// Capture the new matrix and process a difference map
		preProcess(new_cursor);
		
		// Calculate the absolute difference between the two frames
		absdiff(
			processed_[cursor_],
			processed_[new_cursor],
			difference_);
		
		// Update the buffer index
		cursor_ = new_cursor;
	} // void Buffer::step()
	
	Mat Buffer::getRaw()
	{
		return raw_[cursor_];
	} // Mat Buffer::getRaw()
	
	Mat Buffer::getProcessed()
	{
		return processed_[cursor_];
	} // Mat Buffer::getProcessed()
	
	Mat Buffer::getDifference()
	{
		return difference_;
	} // Mat Buffer::getDifference()
}