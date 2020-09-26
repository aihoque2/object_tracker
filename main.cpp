#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <iostream>
#include "dataCollector.h"

using namespace std;
using namespace cv;

int main(){

	string filename = "data/videos/Test21.avi";
	
	dataCollector myCollector(filename);

	myCollector.run();

	return 0;

}
