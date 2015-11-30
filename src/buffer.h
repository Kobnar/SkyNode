#ifndef SKYNODE_BUFFER_H_
#define SKYNODE_BUFFER_H_

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
		static const int PREPROCESS_BLUR_ = 2;
		static const int BUFFER_LENGTH_ = 2;
		
		// Members:
		VideoCapture source_;
		Mat raw_ [BUFFER_LENGTH_];
		Mat processed_ [BUFFER_LENGTH_];
		Mat difference_;
		bool cursor_ = 0;
		
		// Private methods:
		void preProcess(int idx);
		
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
		
	}; // class Buffer
	
}

#endif // SKYNODE_BUFFER_H_