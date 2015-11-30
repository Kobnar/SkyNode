#include "node.h"

using namespace std;
using namespace cv;
namespace skynode {
	
	Node::Node(VideoCapture capture_source)
	{
		// Copy the principal capture source
		source_ = capture_source;
		
		// Read the raw frame
		// TODO: Create a special frame object
		source_.read(raw_matrix_);
	} // Node::Node(VideoCapture capture_source)
	
	// Cycles the Node forward
	void Node::step()
	{
		// Capture a new raw frame
		// TODO: Create a special frame object
		source_.read(raw_matrix_);
	}
	
	Mat Node::getRaw()
	{
		return raw_matrix_;
	}

} // namespace skynode