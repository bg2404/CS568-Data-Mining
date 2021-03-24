#include<fstream>
#include<iostream>
#include<vector>


#include "Relation.h"
#include "ReadInput.h"

using namespace std;


int main(int argc, char**argv) {

	if(argc != 3) {
        	cout << "Usage: ./test {file} {update file}\n";
		exit(1);
	}

	string file = argv[1];
	string updateFile = argv[2];

	// read both dataset and update file
	ReadInput reader1(file);
	Relation<double> dataBase = reader1.read();
	ReadInput reader2(updateFile);
	Relation<double> updates = reader2.read();


	// make a set of all the transactions
	set<vector<double>> s;
	for(auto x : dataBase)
		s.insert(x);

	int cnt = 1;
	for(auto update : updates) {
		int type = update.back();
		update.pop_back();

		if(type == 1) {
			dataBase.push_back(update);
			s.insert(update);

			// create a new dataset with the point inserted
			fstream fout;
			int n = file.length();
			string name = file.substr(0, n - 4) + "__" + to_string(cnt) + ".csv";
			fout.open(name, ios::out);

			for(auto x : dataBase) {
				if(s.find(x) != s.end()) {
					for(auto y : x)
						fout << y << ' ';
					fout << '\n';
				}
			}

			fout.close();
		}
		else {
			s.erase(update);

			// create a new dataset with the point deleted
			fstream fout;
			int n = file.length();
			string name = file.substr(0, n - 4) + "__" + to_string(cnt) + ".csv";
			fout.open(name, ios::out);

			for(auto x : dataBase) {
				if(s.find(x) != s.end()) {
					for(auto y : x)
						fout << y << ' ';
					fout << '\n';
				}
			}
			
			fout.close();
		}
		cnt++;
	}

	
	return 0;
}
