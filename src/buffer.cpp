#include "buffer.h"

using namespace std;
using namespace cv;
namespace skynode {
	
	// Primary constructor.
	Buffer::Buffer(VideoCapture capture_source)
	{
		// Save the VideoSource object
		source_ = capture_source;
		
		// Capture and pre-process the initial matrix
		source_.read(raw_[cursor_]);
		preProcess(cursor_);
		
		// Create an empty difference matrix
		difference_ = Mat::zeros(
			raw_[cursor_].rows,
			raw_[cursor_].cols,
			CV_8UC1);
		
	} // Buffer::Buffer(VideoCapture capture_source)
	
	// Pre-processes a raw image matrix.
	void Buffer::preProcess(int idx)
	{
		// Blur the matrix
		blur(
			raw_[idx],
			processed_[idx],
			Size(PREPROCESS_BLUR_, PREPROCESS_BLUR_));
		
		// Convert the matrix to grayscale
		cvtColor(
			processed_[idx],
			processed_[idx],
			COLOR_BGR2GRAY);
	} // void Buffer::preProcess(int idx)
	
	// Cycles the buffer by calculating a new cursor, capturing a new image matrix,
	// pre-proccessing the new matrix and calculating an absolute difference between
	// the new matrix and the old one.
	void Buffer::step()
	{
		// Calculate the new matrix' index
		int new_cursor = (cursor_ + 1) % BUFFER_LENGTH_;
		
		// Capture and pre-process a new image matrix
		source_.read(raw_[new_cursor]);
		preProcess(new_cursor);
		
		// Calculate the absolute difference between the two frames
		absdiff(
			processed_[cursor_],
			processed_[new_cursor],
			difference_);
		
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
}