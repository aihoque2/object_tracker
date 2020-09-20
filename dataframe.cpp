 #include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <math.h>
#include "dataframe.h"

#define PI 3.14159265


using namespace std;


Dataframe::Dataframe(){
	vector<pair<string, vector<double>>> myData;
	_data = myData;	

}

Dataframe::Dataframe(string filename){

	readFile(filename);
		
}


void Dataframe::readFile(string filename){
	
	ifstream myFile(filename);
	
	if(!myFile.is_open()) throw std::runtime_error("Could not open file");

	string line;

	//initialize the header of each column in the dataframe
	vector<string> colNames = {"seconds", "latitude", "longitude", "altitude"};
	for (int i =0; i < colNames.size(); i++){
		_data.push_back({colNames[i], vector<double>{}});
	}

	double val;
	while(getline(myFile, line)){
		std::stringstream ss(line);

		int colIdx = 0;

		while(ss >> val){
			_data[colIdx].second.push_back(val);

			if (ss.peek() == ',') ss.ignore();

			colIdx++;
		}

	}
	//close file
	myFile.close();
}



void Dataframe::writeToFile(string filename){
	/*create a csv file representing our dataframe
	 * INPUT: name of the file
	 * create a csv file to be stored 
	 * in the "results" directory */
	
	ofstream myFile(filename);
	
	//add the column names
	for (int i = 0; i < _data.size(); i++){
		myFile << _data.at(i).first;
		if (i != _data.size() -1) myFile << ",";
	
	}	

	myFile << "\n";

	//send data to the stream
	for (int i = 0; i < _data.at(0).second.size(); i++){ //traverse the columns
		
		for (int j = 0; j < _data.size(); j++){ //traverse the different rows
			
			myFile << _data.at(j).second.at(i);
			if(j != _data.size() - 1) myFile << ",";

		}
	
		myFile << "\n";
	}
	
	myFile.close();
}


void Dataframe::addColumn(pair<string, vector<double>> column){
	/*insert the given column into _data
	 * input: the column in form of std::pair
	 * the pair contains the column name (string) and 
	 * the data of the column (vector<long double>)*/

	_data.push_back(column);


}

vector<pair<string, vector<double>>> Dataframe::getData(){
	return _data;
}


	
vector<double> Dataframe::getTime(){
	return _data[0].second;
}

