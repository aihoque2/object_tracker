#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "process_images.cpp"
#include <iostream>

using namespace std;
using namespace cv;
/*
Mat process_division(Mat image){                                                                      
        //note: this takes in 1-channel images for input                                              
        Mat gray, smooth, dst;                                                                        
                                                                                                      
        //cvtColor(image, gray, CV_BGR2GRAY);// gray in case of emergency                                  
                                                                                                      
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));                                    
                                                                                                      
        morphologyEx(image, smooth, MORPH_DILATE, kernel); //bring out everything                     
                                                                                                      
        divide(image, smooth, dst, 255.0);                                                            
                                                                                                      
        return dst;                                                                                   
                                                                                                      
}                                                                                                     
*/                                                                                                    

int main(){

	Mat image = imread("data/images/Test21_1.tif", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data){
		cout <<"could not open image" << endl;
		return -1;
	}

	//Mat divided = process_division(image);
	Mat divided = processRectDivision(image);
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", divided);

	waitKey(0);
	return 0;

}
