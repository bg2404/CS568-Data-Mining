#include<fstream>
#include<iostream>
#include<vector>


#include "Relation.h"
#include "ReadInput.h"

using namespace std;


int main(int argc, char**argv) {
	if(argc != 3) {
		cout << "Invalid output format\n";
		exit(1);
	}

	string file = argv[1];
	string updateFile = argv[2];

	ReadInput reader(file);
	Relation<double> dataBase = reader.read();
	ReadInput reader2(updateFile);
	Relation<double> updates = reader2.read();


	set<vector<double>> s;
	for(auto x : dataBase)
		s.insert(x);

	int cnt = 1;
	for(auto update : updates) {
		update.pop_back();
		dataBase.push_back(update);
		
		fstream fout;
		int n = file.length();
		string name = file.substr(0, n - 4) + "__" + to_string(cnt) + ".csv";
		fout.open(name, ios::out);
		for(auto x : dataBase) {
			for(auto y : x)
				fout << y << ' ';
			fout << '\n';
		}
		fout.close();
		cnt++;
	}

	/*
	int cnt = 1;
	for(auto update : updates) {
		update.pop_back();
		s.erase(update);

		fstream fout;
		int n = file.length();
		string name = file.substr(0, n - 4) + "_" + to_string(cnt) + ".csv";
		fout.open(name, ios::out);
		for(auto x : dataBase) {
			if(s.find(x) != s.end()) {
				for(auto y : x)
					fout << y << ' ';
				fout << '\n';
			}
		}
		fout.close();
		cnt++;
	}
	*/
	
	return 0;
}
