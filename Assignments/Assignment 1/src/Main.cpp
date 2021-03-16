#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "Cluster.h"
#include "DBSCAN.h"
#include "ReadInput.h"
#include "Relation.h"
#include "SUBCLU.h"
#include "Subspace.h"
#include "INCRDBSCAN.h"
#include "INCRSUBCLU.h"

using namespace std;

map<vector<double>, int> dbids;
map<int, int> idc;

int main(int argc, char **argv) {
    if (argc != 4) {
        if (argc == 2) {
            string help = argv[1];
            if (help == "help") {
                cout << "min points and epsilon values that work....\n";
                cout << "mouse.csv 12 0.05\n";
                cout << "cube.csv  1   70\n";
                cout << "Iris-Unlabeled.csv 4 0.4\n";
            }
        }
        cout << "Usage: ./subclu {file} {min points} {epsilon}\n";
        return 0;
    }

    string file = argv[1];
    int mnPnts = stoi(argv[2]);
    double epsilon = stod(argv[3]);

    SUBCLU subclu(file, mnPnts, epsilon);

    cout << "Starting SUBCLU run with file = " << file << " ,mnPnts = " << mnPnts << " ,epsilon = " << epsilon << " .....\n";
    map<Subspace, vector<Cluster>> result = subclu.run();


    // Testing and Visualisation
    ReadInput reader(file);
    Relation<double> dataBase = reader.read();

    if (!dataBase.size())
        return 0;


    for (int i = 0; i < (int)(dataBase.size()); i++) {
        dbids[dataBase[i]] = i;
    }

    // printing Subspace*.csv files
    for (auto clustering : result) {
        string file = "Subspace";
        Subspace subspace = clustering.first;
        for (auto d : subspace.getDimensions()) {
            file.push_back((char)(d + '0'));
        }
        file = file + ".csv";

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

    // incremental subclu
    INCRSUBCLU incrsubclu(file, "test_incr.txt", mnPnts, epsilon);
    incrsubclu.run();


    return 0;
}
