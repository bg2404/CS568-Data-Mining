#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "Cluster.h"
#include "Subspace.h"
#include "SUBCLU.h"
#include "INCRSUBCLU.h"

using namespace std;

int main(int argc, char **argv) {
    // Arguments Parsing
    if (argc != 5) {
        if (argc == 2) {
            string help = argv[1];
            if (help == "help") {
                cout << "min points and epsilon values that work....\n";
                cout << "mouse.csv 12 0.05\n";
                cout << "cube.csv  1   70\n";
                cout << "Iris-Unlabeled.csv 4 0.4\n";
            }
        }
        cout << "Usage: ./subclu {file} {update file} {min points} {epsilon}\n";
        return 0;
    }
    string file = argv[1];
    string updateFile = argv[2];
    int mnPnts = stoi(argv[3]);
    double epsilon = stod(argv[4]);


    // Running SUBCLU
    SUBCLU subclu(file, mnPnts, epsilon);
    // cout << "Starting SUBCLU run with file = " << file << " ,mnPnts = " << mnPnts << " ,epsilon = " << epsilon << " .....\n";
    map<Subspace, vector<Cluster>> result = subclu.run();



    // Printing Subspace*.csv files
    for (auto clustering : result) {
	
	// creating file name
        string file = "Subspace";
        Subspace subspace = clustering.first;
        for (auto d : subspace.getDimensions()) {
            file.push_back((char)(d + '0'));
        }
        file = file + ".csv";

	// Opening file and writing the Clusters found
        fstream p_file;
        p_file.open(file, ios::out);


        for (auto cluster : clustering.second) {
		int clusterId = cluster.getClusterId();
		int noise = cluster.isNoise();
		map<int, int> ids = cluster.getIds();
		int split = cluster.getSplit();
		p_file << clusterId << ' ' << noise << ' ' << split << ' ' << ids.size() << '\n';
		for(pair<int, int> id : ids) {
			p_file << id.first << ' ' << id.second << '\n';
		}

        }
        p_file.close();
    }


    
    // Running INCRSUBCLU
    // cout << "###########################################################################################################\n";
    // cout << "Starting INCRSUBCLU run with file = " << file << " ,mnPnts = " << mnPnts << " ,epsilon = " << epsilon << " .....\n";
    
    INCRSUBCLU incrsubclu(file, updateFile, mnPnts, epsilon);
    incrsubclu.run();
    incrsubclu.print();
    incrsubclu.newDatabase();
  

    /*
    // Printing Incremental Results
    incrsubclu.print();
    */
    return 0;
}
