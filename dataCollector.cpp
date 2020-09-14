#include "dataCollector.h"
#include "process_images.cpp"

using namespace std;
using namespace cv;


dataCollector::dataCollector(const string filename){
	createCollector(filename);
}

void dataCollector::createCollector(const string filename){
	videoCapture capture(filename);	
	Mat img, imgGray, imgDiv;   
	capture>>img;

	cvtColor(img, imgGray, COLOR_BGR2GRAY); //convert to grayscale to process the image
	
	this.filename = filename;
	this.boundingBoxes = detectObjects(imgGray);
	this.corners = getCorners(imgGray);

}


vector<Point2f> getCorners(Mat image){
        
	/* return an image that shows all the corners detected 
         * utilizes the goodFeaturesToTrack() function*/

        //shi-tomasi parameters


        vector<Point2f> corners; //our return
        double qualityLevel = 0.01;
        double minDistance = 10;
        int blockSize = 3;
        bool useHarrisDetector = false;
        double k = 0.04;
        int maxCorners = 300;

        Mat div = process_division(image);

        goodFeaturesToTrack(div,
               corners,
               maxCorners,
               qualityLevel,
               minDistance,
               Mat(),
               blockSize,
               useHarrisDetector,
               k);

        return corners;	
	

}


Rect dataCollector::circle_to_square(int x, int y, int r){ //fun fact: openCV has a Rect_ class for rectangles
        /*create a rectangle from the given 
        * circle ands its center points
        * radius size for the square is 
        * gonna be like 60 or 70 */

        int cornerX = x - 50;
        int cornerY = y - 50;
        Point corner(cornerX, cornerY);
        Size squareDim(100,100);

        Rect ret(corner, squareDim);

        return ret;


}



vector<Rect> dataCollector::detectObjects(Mat image){
        /*take grayscale image and return
         * predicted bounding boxes in the valid area
	 *
	 * input: 1-channel image
         * output: a vector containing the circles
         * */

        vector<Vec3f> circles;
        vector<Rect> boxes;

        int boxRadius = 50; //parameter might need tuning


        Mat div = process_division_circle(image);

        //get our circles
        HoughCircles(div, circles, CV_HOUGH_GRADIENT, 1, 150, 255, 45, 0, 60);

        for (int i = 0; i < circles.size(); i++){
                //check for any points out of bounds
                if (circles[i][0] < 100 || circles[i][0] >900){
                        circles.erase(circles.begin() + i);
                }
                else if (circles[i][1] < 100 || circles[i][1] >900){
                        circles.erase(circles.begin() + i);
                }

                //normal case
                else{
                        Rect toPush = circle_to_square(cvRound(circles[i][0]), cvRound(circles[i][1]), boxRadius);
                        boxes.push_back(toPush);

                }

        }

        return boxes;
	



}



void dataCollector::run(){	
	string myFile = dataCollector.filename
}
