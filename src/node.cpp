#include "node.h"

using namespace std;
using namespace cv;
namespace skynode {
	
	Node::Node(VideoCapture capture_source)
	{
		buffer_ = Buffer(capture_source);
	} // Node::Node(VideoCapture capture_source)
	
	// Cycles the Node forward one step
	void Node::step()
	{
		// Cycle the buffer forward one step
		buffer_.step();
	}
	
	// Return the most recent raw matrix
	Mat Node::getRaw()
	{
		return buffer_.getRaw();
	}
	
	// Return the most recent debug matrix
	Mat Node::getDebug()
	{
		return buffer_.getDebug();
	}

} // namespace skynode