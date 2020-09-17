/*using my image processing functions 
 * written in python for C++ implementation */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


namespace localHelpers{
Mat processRectDivision(Mat image){
	//note: this takes in 1-channel images for input
	Mat gray, smooth, dst;

	//cvtColor(image, gray, BGR2GRAY);// gray in case of emergency
	
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
	
	morphologyEx(image, smooth, MORPH_DILATE, kernel); //bring out everything

	divide(image, smooth, dst, 255.0);

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

}
