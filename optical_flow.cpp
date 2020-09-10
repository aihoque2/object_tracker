/*optical_flow.cpp
 * I'm just experimenting with some corner
 * detection and tracking with C++ here
 */

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace cv;
using namespace std;


Mat process_division(Mat image){
        //note: this takes in 1-channel images for input
        Mat gray, smooth, dst;

        //cvtColor(image, gray, CV_BGR2GRAY);// gray in case of emergency

        Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));

        morphologyEx(image, smooth, MORPH_DILATE, kernel); //bring out everything

        divide(image, smooth, dst, 255.0);

        return dst;

}


vector<Point2f> get_corners(Mat image){ //1 channel image will do
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
               k );

	return corners;

	
}


int main(){

	string filename = "data/images/Test21_1.tif";
	Mat div, copy;	

	Mat image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data){
		cout << "ur image sux" << endl;
		return -1;
	}
	

	vector<Point2f> corners = get_corners(image);
	 
	copy = image.clone();
	
	int r = 4;

	for( int i = 0; i < corners.size(); i++ ){ 
	circle( copy, corners[i], r, Scalar(255, 0, 0), -1, 8, 0 );

	
	}

	namedWindow("Display Window", WINDOW_AUTOSIZE);
	imshow("Display Window", copy);
	waitKey(0);
	return 0;
}

