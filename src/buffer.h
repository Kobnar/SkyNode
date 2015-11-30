#ifndef SKYNODE_BUFFER_H_
#define SKYNODE_BUFFER_H_

#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;
namespace skynode {
	
	// Captures and processes image matrices from a given VideoCapture object
	// so they can be used with higher level "spot and track" logic in a
	// parent Node.
	class Buffer
	{
	private:
		// Settings:
		// 	TODO: Fetch settings from file/args
		static const int BUFFER_LENGTH_ = 2;
		static const Size PREPROCESS_BLUR_;
		static const int MIN_DIFF_THRESHOLD_;
		static const Size NOISE_ERODE_;
		
		// Members:
		VideoCapture source_;
		Size matrix_size_;
		Mat raw_ [BUFFER_LENGTH_];
		Mat processed_ [BUFFER_LENGTH_];
		Mat difference_;
		Mat debug_;
		bool cursor_ = 0;
		
		// Private methods:
		void preProcess(int idx);
		void genDifference(int idx);
		void detectMovement();
		
	public:
		// Constructors:
		Buffer() {};
		Buffer(VideoCapture capture_source);
	
		// The main "step" method to cycle a new frame:
		void step();
	
		// Interface for class members:
		Mat getRaw();
		Mat getProcessed();
		Mat getDifference();
		Mat getDebug();
		
	}; // class Buffer
	
}

#endif // SKYNODE_BUFFER_H_