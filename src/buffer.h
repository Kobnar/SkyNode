#ifndef SKYNODE_BUFFER_H_
#define SKYNODE_BUFFER_H_

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;
namespace skynode {
	
	//	The main frame processor and buffer
	// Designed to optimize effective memory management techniques
	// while processing each frame from the capture source
	class Buffer
	{
	private:
		// TODO: settings from file/args
		static const int PREPROCESS_BLUR_ = 2;
		static const int BUFFER_LENGTH_ = 2;
		
		VideoCapture source_;
		Mat raw_ [BUFFER_LENGTH_];
		Mat processed_ [BUFFER_LENGTH_];
		Mat difference_;
		bool cursor_ = 0;
		
		void preProcess(int idx);
		
	public:
		Buffer() {};
		Buffer(VideoCapture capture_source);
	
		void step();
	
		Mat getRaw();
		Mat getProcessed();
		Mat getDifference();
		
	}; // class Buffer
	
}

#endif // SKYNODE_BUFFER_H_