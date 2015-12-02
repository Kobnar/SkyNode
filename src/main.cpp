#include "main.h"

using namespace std;
using namespace cv;

//	The main entry point of the program.
// TODO: Accept parameters (e.g. settings file, source file, etc.)
int main()
{
	// Define a new window
	namedWindow("SkyNode", 1);
	
	// Define a capture source
	// TODO: Allow video source file
	VideoCapture main_source(0);
	
	// Instantiate the new node
	skynode::Node node(main_source);
	
	while(true)
	{
		// Cycle the frame
		node.step();
		
		// Draw the most recent raw frame in the main window
		imshow("SkyNode", node.getDebug());
		
		// Wait for user input
		// TODO: Actually handle input
		waitKey(10);
	}
}