/*dataCollector.h
 *
 * this is the .h for our detector pipeline that
 * will collect the displacement data in the
 * video datasets */

#include <opencv2/highgui.hpp>
#include <opencv2/imageproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <process_images.h>


using namespace cv;
using namespace std;

class dataCollector{


	public:
		dataCollector(string filename);
		
		
		/*in case someone declares
                 * default constructor, they
                 * can set the member variables
                 * with this func*/
		void createCollector(const string filename); //the function that constructs our dataCollector object

		vector<Point2f>getCorners(Mat image);
	
			
		vector<Rect> detectObjects(Mat image); //detect our objects and return bounding boxes

		void sortFn();
		void sortBoundingBoxes(); //sort our bounding boxes after gathering enerything.
	

		Rect circle_to_square(int x, int y, int r); //I use houghCircles() for object detection, and I create a bounding box

			

		void run(); //this is where the tracking algorithm is run
	
	
	private:
		string filename; //this is important data. protecc it at all times
		Vector<Point2f> corners;
		vector<Rect> boundingBoxes;
};
