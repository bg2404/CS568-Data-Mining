#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "Cluster.h"
#include "Subspace.h"
#include "SUBCLU.h"
#include "INCRSUBCLU.h"
#include "ReadInput.h"
#include "Relation.h"

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

    for(int i = 1; i <= 200; i++) {
	    int n = file.size();
	    string temp = file.substr(0, n - 4) + "__" + to_string(i) + ".csv";
	    SUBCLU subclu(temp, mnPnts, epsilon);
	    result = subclu.run();
    }
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

    
    

    /*
    ReadInput reader(updateFile);
    Relation<double> updates = reader.read();


    fstream fout;
    fout.open(file, ios::app);
    for(auto update: updates) {
	    update.pop_back();

	    for(auto x : update) {
		    fout << x << ' ';
	    }
	    fout << '\n';
    }
    fout.close();

    SUBCLU subclu2(file, mnPnts, epsilon);
    result = subclu2.run();
    

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
    */

    // Running INCRSUBCLU
    // cout << "###########################################################################################################\n";
    // cout << "Starting INCRSUBCLU run with file = " << file << " ,mnPnts = " << mnPnts << " ,epsilon = " << epsilon << " .....\n";
    //INCRSUBCLU incrsubclu(file, updateFile, mnPnts, epsilon);
    //incrsubclu.run();
    //incrsubclu.print();

    /*
    // Printing Incremental Results
    incrsubclu.print();
    */
    return 0;
}
