/*dataCollector.h
 *
 * this is the .h for our detector pipeline that
 * will collect the displacement data in the
 * video datasets */

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>


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

		vector<Point2f>getCorners(Mat image); //get all the trackable corners in the image

		vector<Point2f> getPointsWithinBox(Rect boundingBox); //get the subset of corners that are within the rectangle
	
			
		vector<Rect> detectObjects(Mat image); //detect our objects and return bounding boxes

		//bool compareRects(Rect leftRect, Rect rightRect); //our function to pass through std::sort()

		void sortBoundingBoxes(); //sort our bounding boxes after gathering everything
	

		Rect circle_to_square(int x, int y, int r); //I use houghCircles() for object detection, and I create a bounding box

			

		void run(); //this is where the tracking algorithm is run
	
	
	private:
		//member variables
		string filename; //this is important data. protecc it at all times
		vector<Point2f> corners; //these points are our initial coordinates where the muscle well starts at. detect displacement by subracting these
		vector<Rect> boundingBoxes;

		//member functions for image processing
		Mat processRectDivision(Mat image);
		Mat processCircDivision(Mat image);
};
