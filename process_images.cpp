/*using my image processing functions 
 * written in python for C++ implementation */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;






Mat process_division(Mat image){
	//note: this takes in 1-channel images for input
	Mat gray, smooth, dst;

	cvtColor(image, gray, BGR2GRAY);// gray in case of emergency
	
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
	
	morphologyEx(image, smooth, MORPH_DILATE, kernel); //bring out everything

	divide(image, smooth, dst, 255.0);

	return dst;

}


