/*dataCollector.cpp */

#include "dataCollector.h"

using namespace std;
using namespace cv;

/*use of the private heleprs*/
Mat dataCollector::processRectDivision(Mat image){


	//note: this function takes in a 1-channel image
        Mat smooth, dst;

        //cvtColor(image, gray, BGR2GRAY);// gray in case of emergency

        Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));

        morphologyEx(image, smooth, MORPH_DILATE, kernel); //bring out everything

        divide(image, smooth, dst, 255.0);

        return dst;


}


Mat dataCollector::processCircDivision(Mat image){

                Mat smooth, div;

                Mat rectKernel = getStructuringElement(MORPH_RECT, Size(5,5));
                Mat circKernel = getStructuringElement(MORPH_ELLIPSE, Size(5,5));

                morphologyEx(image, smooth, MORPH_DILATE, rectKernel);
                medianBlur(smooth, smooth, 5);
                morphologyEx(smooth, smooth, MORPH_DILATE, circKernel);

                divide(image, smooth, div, 255.0);

                return div;


}


/*now for the public functions*/



/*our constructor*/
dataCollector::dataCollector(string filename){
	createCollector(filename);
}


/*constructor helper*/
void dataCollector::createCollector(string filename){
	VideoCapture capture(filename);
	if (!capture.isOpened()){
		cout << "unable to open video file in constructor" << endl;;
		return;
	}

	Mat img, imgGray, imgDiv;
	capture >> img;

	cvtColor(img, imgGray, COLOR_BGR2GRAY); //convert to grayscale to process the image

	filename = this->filename;
	boundingBoxes = detectObjects(imgGray);
	sortBoundingBoxes();
	corners = getCorners(imgGray);

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

        Mat div = processRectDivision(image);

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
	for (int i = 0; i < corners.size(); i++){

		if (boundingBox.contains(corners[i])){
			ret.push_back(corners[i]);
		}
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


        Mat div = processCircDivision(image);

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




bool compareRects(Rect leftRect, Rect rightRect){

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

        for( int i = 0; i < boundingBoxes.size(); i++ )
        {

                if(i%2 == 0) {
			if (boundingBoxes[i].x > boundingBoxes[i+1].x){
				//doing this swap to follow the labeling required by my PhD I work under
                        	Rect temp = boundingBoxes[i];
                        	boundingBoxes[i] = boundingBoxes[i+1];
                        	boundingBoxes[i+1] = temp;
			}
                }


        }
}


/*helper function to get average of values in vector*/
double dataCollector::get_mean(vector<double> vec){
	double sum = 0.0;
	int n = vec.size();

	for (int i = 0; i < n; i++){
		//TODO
		sum += abs(vec[i]);

	}

	double ret = sum/(double)n;
	return ret;


}

void dataCollector::run(string myFilename){
	//TODO: this function

	VideoCapture capture(myFilename);
	if (!capture.isOpened()){
		cout << "ur video sux" << endl;
	}


	Mat firstImg, oldGray, oldDiv; //oldDiv to be used in video loops
	capture >> firstImg;

	cout <<"turning oldGray" << endl;
	cvtColor(firstImg, oldGray, COLOR_BGR2GRAY);


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

	bool timeAdded = false; //flag to tell us if we added the time column to our recording


	for (int i = 0; i < boundingBoxes.size(); i++){
		//for each rect, play through the video and capture
		//this could maybe made faster with the use of OpenMP and parallel programming
		//maybe use threads aswell

		VideoCapture capture(myFilename);

		//subsetCorners is passed through calcOpticalFlowPyrLK(). firstCorners holds x_0 and y_0 to record our displacement
		vector<Point2f> initialCorners = getPointsWithinBox(boundingBoxes[i]);
		vector<Point2f> subsetCorners = getPointsWithinBox(boundingBoxes[i]);


		Mat mask = Mat::zeros(firstImg.size(), firstImg.type()); //we want multiple colors when presenting the mask, so we use the firstImg

		vector<Point2f> p1; //holder vec to get the next neighbors in optical flow calculation

		//vectors to hold the dataframe values
		vector<double> timeVec; //record timestamp
		vector<double> avgDisplacementVec; //average displacement at the timestamp for each square

		while(true){
			vector<double> deltaX; //the change in x for each point at index 'i' of subsetCorners


			Mat frame, frameGray, frameDiv;
			capture >> frame;
			if(frame.empty()){
				break;
			}
			cvtColor(frame, frameGray, COLOR_BGR2GRAY);
			oldDiv = processRectDivision(oldGray);
			frameDiv = processRectDivision(frameGray);

			//calculate optical flow
			vector<uchar> status;
			vector<float> err;

			TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
			calcOpticalFlowPyrLK(oldDiv, frameDiv, subsetCorners, p1, status, err, Size(15,15), 2, criteria);

			bool toRecord = false; //this flag tells us when to record data


			int timestamp = capture.get(CAP_PROP_POS_MSEC);//get the time

			//go through corners to draw mask on and record data
			vector<Point2f> good_new;
			for (int i = 0; i < subsetCorners.size(); i++){
				if (status[i] == 1){
					good_new.push_back(p1[i]);
					line(mask, subsetCorners[i], p1[i], colors[i], 2);
					circle(frame, p1[i], 5, colors[i], -1);
				}

				if ((timestamp % 250) == 0){
					toRecord = true;
					double dx = (double)(p1[i].x - initialCorners[i].x);
					deltaX.push_back(dx); //put all corners' displacements in this vector
				}

			}

			if (toRecord){
				//recording data
				double avgDisplacement = get_mean(deltaX);
				avgDisplacementVec.push_back(avgDisplacement);
				timeVec.push_back(timestamp);

				//resetting variables
				deltaX.clear();
				toRecord = false;
			}


			Mat img;
			add(frame, mask, img);
			rectangle(img, boundingBoxes[i], Scalar(0,0,255), 1, 8, 0);
			
			//adding text
			string textString = "object_" + to_string(i+1);
			Point2f rectPoint(boundingBoxes[i].x, boundingBoxes[i].y-5.0);
			putText(img, textString, rectPoint, cv::FONT_HERSHEY_DUPLEX, 0.7, CV_RGB(255, 0, 0), 2);

			imshow("frame", img);

			int keyboard = waitKey(30);
			if (keyboard == 'q' || keyboard ==27){break;}

			oldGray = frameGray.clone();
		} //while loop for video ends here

		//TODO: record the data here to dataFrame()
		if (!timeAdded){
      cout << "reached the time column condition" << endl;
			string timeString = "time (ms)";
			pair<string, vector<double>> timeCol(timeString, timeVec);
			myDF.addColumn(timeCol);
			timeAdded = true;
		}

		pair<string, vector<double>> column("displacement_" + to_string(i+1), avgDisplacementVec);
		myDF.addColumn(column);
	}

}


void dataCollector::export_csv(string outFilename){
	myDF.writeToFile(outFilename);

}

