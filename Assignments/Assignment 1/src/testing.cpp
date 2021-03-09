#include <iostream>
#include <set>
#include <vector>
#include <fstream>

#include "DBSCAN.h"
#include "Cluster.h"
#include "ReadInput.h"
#include "Subspace.h"
#include "Relation.h"
#include "INCRDBSCAN.h"

using namespace std;
Relation<double> dataBase;
map<vector<double>, int> dbids;


int main(int argc, char **argv)
{
    string file = argv[1];
	int mnPnts = stoi(argv[2]);
	double epsilon = stod(argv[3]);
    string file_incr = argv[4];

    //read file
	ReadInput reader(file);
	dataBase = reader.read();

    // generating dbids
    for (int i = 0; i < (int)(dataBase.size()); i++)
	{
		dbids[dataBase[i]] = i;
	}

    //create subspace
    vector<int> dimensions;
    dimensions.push_back(1);
    dimensions.push_back(1);
    dimensions.push_back(1);
    Subspace subspace = Subspace(dimensions);

    //run DBSCAN
    DBSCAN dbscan(dataBase,subspace,epsilon,mnPnts,dbids);
    vector<Cluster> clustering = dbscan.getClusters();
    subspace.setClusters(clustering);
    subspace.print();

    //get points to add
    Relation<double> AddPoints;
    ReadInput reader2(file_incr);
    AddPoints = reader2.read();
    int id = dataBase.size();

    //for each point call IncrDBSCAN
    for(int i=0;i<AddPoints.size();i++)
    {
        vector<double> point = AddPoints[i];
        INCRDBSCAN incrdbscan = INCRDBSCAN(point,epsilon,mnPnts,dataBase,subspace,id,dbids);
        subspace = incrdbscan.Insert();
        dataBase.push_back(point);
        dbids.insert(make_pair(point,id));
        id++;
        subspace.print();
    }    
}