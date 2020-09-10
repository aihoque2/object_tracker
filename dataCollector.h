/*dataCollector.h
 *
 * this is the .h for our detector pipeline that
 * will collect the displacement data in the
 * video datasets */

#include <opencv2/highgui.hpp>
#include <opencv2/imageproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

class dataCollector{


	public:
		dataCollector(string filename);
		void setFilename(string filename); /*in case someone declares the
						     default constructor, they can just
						     set the filename with this setter func*/
		
		//our image processing gets run here
		Mat processRectDivision(Mat image); //bring out the squares
		Mat processCircDivision(Mat image); //bring out the circles
		vector<Vec3f> detectCircles; //detect circles for our function

		
		

		void run(); //this is where the tracking algorithm is run
	
	
	private:
		string filename //this is important data. protecc it at all times
		vector<Point2f> boundingBoxes	
};
