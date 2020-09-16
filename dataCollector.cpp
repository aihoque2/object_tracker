/*dataCollector.cpp */

#include <opencv2/highgui.hpp>                                                                        
#include <opencv2/imgproc.hpp>                                                                        
#include <opencv2/videoio.hpp>                                                                        
#include <opencv2/video.hpp>                                                                          
#include <iostream>
#include <stdlib.h>                                                                                   
#include <stdio.h>                                                                                    
#include <vector>

#include "dataCollector.h"
#include "process_images.cpp"
#include "dataframe.h"

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


vector<Point2f> dataCollector::getCorners(Mat image){
        
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

vector<Point2f> dataCollector::getPointsWithinBox(Rect boundingBox){
	/* get the subset of corners that are within the Rect
	 * 
	 * Input: the bounding box passed as a Rect
	 * output: a vector containing the subset of points
	*/

	vector<Point2f> ret;
	for (int i = 0; i < this.corners.size(); i++){

		if boundingBox.contains(this.corners[i]) ret.push_back(this.corners[i]);
	}

	return ret;

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




bool dataCollector::compareRects(Rect leftRect, Rect rightRect){
	
	if (leftRect.y == rightRect.y){ return leftRect.x < rightRect.x;}
        return (leftRect.y < rightRect.y);


}


void dataCollector::sortBoundingBoxes(){
	/*after we create our boundingBox 
	 * vector, we sort it to follow 
	 * the proper naming procedure
	 * with the use of this function.
	 */

	sort(boundingBoxes.begin(), boundingBoxes.end(), compareRects);

        for( int i = 0; i < this.boundingBoxes.size(); i++ )
        {

                if(i%2 == 0) {
			if (rectangles[i].x > rectangles[i+1].x){
				//doing this swap to follow the labeling required by my PhD I work under
                        	Rect temp = this.boundingBoxes[i];
                        	this.boundingBoxes[i] = this.boundingBoxes[i+1];
                        	this.boundingBoxes[i+1] = temp;
			}
                }


        }	
}

void dataCollector::run(){	
	//TODO: this function
	
	videoCapture capture(this.filename);
	if (!capture.isOpened()){
		cout << "ur video sux" << endl;
	}

	Dataframe myDF;


	Mat firstImg, oldGray, oldDiv; //oldDiv to be used in video loops
	capture << firstImg;

	cvtColor(firstImg, oldGray, COLOR_BGR2GRAY);

	vector<Point2f> p1; //holder vec to get the next neighbors in optical flow calculation

	bool timeAdded = False; //flag to tell us if we added the time column to our recording


	for (int i = 0; i < this.boundingBoxes.size(); i++){
		//for each rect, play through the video and capture
		//this could maybe made faster with the use of OpenMP and parallel programming
		//maybe use threads aswell
		
		//subsetCorners is passed through calcOpticalFlowPyrLK(). firstCorners holds x_0 and y_0 to record our displacement
		vector<Point2f> subsetCorners, initialCorners = getPointsWithinBox(this.boundingBoxes[i]);
		mat Mask = mat::zeroes(firstImg.size(), firstImg.type()); //we want multiple colors when presenting the mask, so we use the firstImg

		//vectors to hold the dataframe values
		vector<double> timeVec; //record timestamp
		vector<double> avgDisplacementVec; //average displacement at the timestamp for each square

		while(true){
			vector<int> deltaX; //the change in x for each point at index 'i' of subsetCorners


			Mat frame, frameGray, frameDiv;
			capture >> frame;
			if(frame.empty()){
				break;
			}
			cvtColor(frame, frameGray);
			oldDiv = process_division(oldGray);
			frameDiv = process_division(frameGray);

			//calculate optical flow
			vector<uchar> status;
			vector<float> err;

			TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
			calcOpticalFlowPyrLK(oldDiv, frameDiv, subsetCorners, p1, status, err, Size(15,15), 2, criteria);
			
			bool toRecord = False; //this flag tells us when to record data

			timestamp = capture.get(CAP_PROP_POS_MSEC);//get the time


			//go through corners to draw mask on and record data
			vector<Point2f> good_new;
			for (int i = 0; i < subsetCorners.size(); i++){
				if (status[i] == 1){
					good_new.push_back(p1[i]);
					line(mask, subsetCorners[i], colors[i], 2);
					circle(frame, p1[i], 5, colors[i], -1);	
				}	

				if ((timestamp % 250) == 0){
					toRecord = true
					int dx = p1[i].x - initialCorners[i].x;
					deltaX.push_back(dx); //put all corners' displacements in this vector
				}
				
			}

			if (toRecord){
				//recording data
				double avgDisplacement = ((double)accumulate(deltaX)/(double)deltaX.size());
				avgDisplacementVec.push_back(avgDisplacement);
				timeVec.push_back(timestamp)

				//resetting variables
				deltaX.clear(); 
				toRecord = false;
			}	

			Mat img;
			add(frame, mask, img);
			rectangle(img, boundingBoxes[i], Scalar(0,0,255), 1, 8, 0);

			imshow("frame", img);

			int keyboard = waitkey(30);
			if (keyboard == 'q' || keyboard ==27){break;}

			oldGray = frameGray.clone();
		}

		//TODO: record the data here to dataFrame()
		if (!timeAdded){
			myDf.addColumn("time (ms)", time);
			timeAdded = true;
		}

		myDF.addColumn("displacement_" + to_string(i), avgDisplacementVec);
	}	

	//TODO: export the dataframe as a .csv file here
	int idx =  this.filename.find(".");
	string resultsFile = this.filename.substr(0, idx);

	myDF.writeToFile("results/" + filename + ".csv");

}
