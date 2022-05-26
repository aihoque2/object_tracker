#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <string>
#include <iostream>
#include "dataCollector.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv){

	string filename = argv[1];
	string output_name = argv[2];
	

	if (argc != 3){
		cout << "ERROR: no arguments given" << endl;
		cout << "to run collect, run ./collect <video filename for tracking> <dest/filename for csv>" << endl;
		return 0;
	}	

	dataCollector myCollector(filename);
	myCollector.run(filename);
	myCollector.export_csv(output_name);
	return 0;

}
