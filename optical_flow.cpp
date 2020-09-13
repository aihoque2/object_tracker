/*optical_flow.cpp
 * I'm just experimenting with some corner
 * detection and tracking with C++ here
 */

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
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

        Mat kernel = getStructuringElement(MORPH_RECT, Size(7,7));

        morphologyEx(image, smooth, MORPH_DILATE, kernel); //bring out everything

        divide(image, smooth, dst, 255.0);

        return dst;

}


vector<Point2f> get_corners(Mat image){ //1 channel image will do
	/* return an image that shows all the corners detected 
	 * utilizes the goodFeaturesToTrack() function*/

	//shi-tomasi parameters
	

	vector<Point2f> corners; //our return
	double qualityLevel = 0.1;
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
	string vidFile = "data/videos/Test21.avi";

	VideoCapture capture(vidFile);	
	if (!capture.isOpened()){
		cout << "ur video sux" << endl;
		return -1;
	}

	// Create some random colors
	vector<Scalar> colors;
	RNG rng;
	for(int i = 0; i < 100; i++)
	{
		int r = rng.uniform(0, 256);
		int g = rng.uniform(0, 256);
		int b = rng.uniform(0, 256);
		colors.push_back(Scalar(r,g,b));
	}


	Mat firstFrame, oldGray;

	capture >> firstFrame;
	
	cvtColor(firstFrame, oldGray, COLOR_BGR2GRAY);

	vector<Point2f> corners = get_corners(oldGray);
	vector<Point2f> p1;
	
	
	// Create a mask image for drawing purposes
	Mat mask = Mat::zeros(firstFrame.size(), firstFrame.type());

	while (true){	
		Mat frame, frameGray;
		capture >> frame;
		if (frame.empty()){
			break;
		}
		cvtColor(frame, frameGray, COLOR_BGR2GRAY);
		
		//calculate optical flow
		vector<uchar> status;
		vector<float> err;
		TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
		calcOpticalFlowPyrLK(oldGray, frameGray, corners, p1, status, err, Size(15,15), 2, criteria);
		
		vector<Point2f> good_new;
		for (int i = 0; i < corners.size(); i++){
			
			//Select good points
			if (status[i] == 1){
				good_new.push_back(p1[i]);
				//draw tracks
				line(mask, p1[i], corners[i], colors[i], 2);
				circle(frame, p1[i], 5, colors[i], -1);
			}

		}

		Mat img;
		add(frame, mask, img);

		imshow("frame", img);

		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27){ break;}

		oldGray = frameGray.clone();
		corners = good_new;
	}

	return 0;


}

