#include "ReadInput.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Relation.h"
#include "Subspace.h"
#include "Cluster.h"

using namespace std;

std::string trim(string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

ReadInput::ReadInput(string file) {
    inputFile.open(file, ios::in);

    // if (inputFile.is_open())
    //     cout << "Opening File: " << file << " ....\n";
}

ReadInput::~ReadInput() {
    // if (inputFile.is_open())
    // 	cout << "Closing File.....\n";
    inputFile.close();
}

// Assumption space separated File
Relation<double> ReadInput::read() {
    Relation<double> data;
    vector<double> row;
    string line;

    while (inputFile) {
        //read a line
        getline(inputFile, line);
        //trim left and right
        line = trim(line);

        if (line == "" || line == "\n")
            continue;

        //devide into different words
        istringstream ss(line);

        //convert each word to float and add to row
        string word;
        while (ss >> word) {
            row.push_back(stod(word));
        }
        //add to data
        data.push_back(row);
        row.clear();
    }
    return data;
}

Subspace ReadInput::readSubspace(vector<int>& dimensions) {

	Subspace subspace(dimensions);
	string line;
	if(inputFile.is_open()) 
	{
		while (inputFile) {
			//read a line
			getline(inputFile, line);
			//trim left and right
			line = trim(line);

			if (line == "" || line == "\n")
				continue;

			//devide into different words
			istringstream ss(line);

			//convert each word to float and add to row
			string word;
			ss >> word;
			int clusterId = stoi(word);
			ss >> word;
			bool noise = stoi(word);
			ss >> word;
			int split  = stoi(word);
			ss >> word;
			int n = stoi(word);

			map<int, int> ids;
			for(int i = 0; i < n; i++) {
				getline(inputFile, line);
				line = trim(line);

				istringstream ss(line);
				string word;
				ss >> word;
				int id = stoi(word);
				ss >> word;
				int neighCount = stoi(word);

				ids.insert(make_pair(id, neighCount));
			}

			vector<double> mean;
			string name = "Cluster_" + to_string(clusterId);
			Cluster cluster(name, ids, noise, clusterId,mean);
			cluster.setSplit(split);
			subspace.insertCluster(cluster);
		}
	}


	return subspace;
}
