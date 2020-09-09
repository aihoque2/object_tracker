/*optical_flow.cpp
 * I'm just experimenting with some corner
 * detection and tracking with C++ here
 */

#include <opencv2/highgui.hpp>
#include <opencv2/imageproc.hpp>

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


Mat get_corners(Mat Image){ //1-channel image for input
	/* return an image that shows all the corners detected 
	 * utilizes the goodFeaturesToTrack() function*/

	//shi-tomasi parameters
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	
	Mat copy = Image.clone() //Deep copy? of the thing

	goodFeaturesToTrack(Image, corners, 100, qualityLevel, minDistance, blockSize, useHarrisDetector, k);
	
	for (int i = 0; i < corners.size(); i++){
		circle(copy, corners[i], r, Scalar(),( rng.uniform(), rng.uniform(), rng.uniform()), -1, 8, 0);
	}

	return copy

}



int main(){

	string filename = "data/images/Test21_1.tif";
	Mat div, cornerImg;	

	Mat image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data){
		cout << "ur image sux" << endl;
		return -1;
	}

	div = process_division(image);
	cornerImg = get_corners(div)

	namedWindow("Display Window", WINDOW_AUTOSIZE);
	


}
