#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class Dataframe{
	public:
		Dataframe(string filename); //constructor that transforms our dataframe
		void readFile(string filename); //take in a file and construct the Dataframe object



		void writeToFile(string filename); //convert our Dataframe to a csv file named with the desired filename
		
		
		vector<pair<string, vector<long double>>> getData(); //get the private member variable _data
		vector<long double> getTime(); //this function returns the our time column
		
		void addColumn(pair<string, vector<long double>> column);




	private:
		vector<pair<string, vector<long double>>> _data;	 //in our std::pair, the string holds the column name, and the vector<long double> holds our values.
};
