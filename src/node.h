#ifndef SKYNODE_NODE_H_
#define SKYNODE_NODE_H_

// #include "opencv2/core/core.hpp"
// #include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "buffer.h"

using namespace std;
using namespace cv;
namespace skynode {
	
	//	The principal SkyNode object
	// Spots and tracks moving objects according to a collection of predefined settings.
	class Node
	{
	private:
		// Settings:
		// 	TODO: Fetch settings from file/args
	
		// Members:
		Buffer buffer_;
		
	public:
		// Constructors:
		Node();
		Node(VideoCapture capture_source);
		
		// The main "step" method to cycle a new frame:
		void step();
		
		// Interface for class members:
		Mat getRaw();
		Mat getDebug();
		
	}; // class Node

} // namespace skynode

#endif // SKYNODE_NODE_H_