/*dataframe.h
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <math.h>
#include <sstream>

using namespace std;

class Dataframe{
	public:
		Dataframe();
		Dataframe(string filename); //constructor that transforms our dataframe
		void readFile(string filename); //take in a file and construct the Dataframe object



		void writeToFile(string filename); //convert our Dataframe to a csv file named with the desired filename
		
		
		vector<pair<string, vector<double>>> getData(); //get the private member variable _data
		vector<double> getTime(); //this function returns the our time column
		
		void addColumn(pair<string, vector<double>> column);




	private:
		vector<pair<string, vector<double>>> _data;	 //in our std::pair, the string holds the column name, and the vector<long double> holds our values.
};

