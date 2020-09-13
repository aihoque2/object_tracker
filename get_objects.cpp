#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;


Mat process_division_circle(Mat image){
	/*assume 1-channel input
	 *
	 */

	Mat smooth, div;
	
	Mat rectKernel = getStructuringElement(MORPH_RECT, Size(5,5));
	Mat circKernel = getStructuringElement(MORPH_ELLIPSE, Size(5,5));

	morphologyEx(image, smooth, MORPH_DILATE, rectKernel);
	medianBlur(smooth, smooth, 5);
	morphologyEx(smooth, smooth, MORPH_DILATE, circKernel);

	divide(image, smooth, div, 255.0);

	return div;	

}


Rect circle_to_square(int x, int y, int r){ //fun fact: openCV has a Rect_ class for rectangles
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



vector<Rect> get_objects(Mat image){
	/*take grayscale image and return
	 * predicted bounding boxes in the valid area
	 * 
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



int main(){

	string filename = "data/images/Test4_1.tif";
	Mat image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data){
		cout << "ur image sux" << endl;
		return -1;
	}	

	vector<Rect> rectangles = get_objects(image);
	for( size_t i = 0; i < rectangles.size(); i++ )
	{
   		//Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		//int radius = cvRound(circles[i][2]);
		// circle center
   		//circle( image, center, 3, Scalar(0,255,0), -1, 8, 0 );

   		// circle outline
   		//circle( image, center, radius, Scalar(0,0,255), 3, 8, 0 );
 		
		//rectangle outline
		//Rect rect = circle_to_square(center.x, center.y, radius);
		rectangle(image, rectangles[i], Scalar(0,0,255), 1, 8, 0 );
		
	}

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", image);
	
	waitKey(0);
	return 0;
}
