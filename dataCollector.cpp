#include "dataCollector.h"

using namespace std;
using namespace cv;


Mat dataCollector::procesRectDivision(Mat image){
	/*take in a 1-channel grayscale image and 
	 * return the division of the processed vs 
	 * original image
	 *
	 * input: Image that we are trying to process
	 * output: a new processed image using the division() method */ 

        Mat gray, smooth, dst;

        //cvtColor(image, gray, CV_BGR2GRAY);// gray in case of emergency

        Mat kernel = getStructuringElement(MORPH_RECT, Size(7,7));

        morphologyEx(image, smooth, MORPH_DILATE, kernel); //bring out everything

        divide(image, smooth, dst, 255.0);

        return dst;


}

Mat dataCollector::processCircDivision(Mat image){
	/*take in a 1-channel grayscale image and
	 * retunr a division of the processed image
	 * that brings out the circular features
	 * used in detectCircles()*/

}

vector<Vec3f> dataCollector::detectCircles(Mat image){
	

}


void dataCollector::run(){	
	string myFile = dataCollector.filename
}
