/*using my image processing functions 
 * written in python for C++ implementation */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;


Mat processRectDivision(Mat image){
	//note: this takes in 1-channel images for input
	Mat gray, smooth, dst;

	cvtColor(image, gray, CV_BGR2GRAY);// gray in case of emergency
	
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
	
	morphologyEx(gray, smooth, MORPH_DILATE, kernel); //bring out everything

	divide(gray, smooth, dst, 255.0);

	return dst;

}


Mat processCircDivision(Mat image){
	 /*take in a 1-channel grayscale image and
         * return a division of the processed image
         * that brings out the circular features
	  * used in detectCircles()*/

	        Mat smooth, div;

        	Mat rectKernel = getStructuringElement(MORPH_RECT, Size(5,5));
        	Mat circKernel = getStructuringElement(MORPH_ELLIPSE, Size(5,5));

        	morphologyEx(image, smooth, MORPH_DILATE, rectKernel);
        	medianBlur(smooth, smooth, 5);
      		morphologyEx(smooth, smooth, MORPH_DILATE, circKernel);

        	divide(image, smooth, div, 255.0);

        	return div;



}


int main(){

	string filename = "data/images/Test21_1.tif";

	Mat image = imread(filename);
	Mat processed = processRectDivision(image);

	imshow("processed image", processed);
	waitKey(0);
	return 0;
}

