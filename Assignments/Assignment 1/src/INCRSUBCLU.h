#ifndef INCRSUBCLU_H
#define INCRSUBCLU_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

#include "Cluster.h"
#include "DBSCAN.h"
#include "Relation.h"
#include "Subspace.h"

class INCRSUBCLU {
   protected:
    double epsilon;
    int minPnts;
    int minDim;
    Relation<double> dataBase;
    Relation<double> updates;

    // map subspaces to their respective clusters
    map<Subspace, vector<Cluster>> Clusterings;

    // map instances to index
    map<vector<double>, int> dbids;

   public:
    INCRSUBCLU(string databaseFilename, string updatesFilename, int minpnts, double epsilon, int mindim = 1);
    map<Subspace, vector<Cluster>> run();
    vector<Cluster> runINCRDBSCAN(Subspace &currSubspace, set<int> &ids);
    vector<Subspace> generateSubspaceCandidates(vector<Subspace> &subspaces);
    Subspace besttSubspace(vector<Subspace> &subspaces, Subspace &candidate);
    bool checkLower(Subspace &candidate, vector<Subspace> &subspaces);
    int getSubspaceData(int dim, Relation<double> &database, vector<Cluster> &clusterData, Subspace &subspace, map<vector<double>, int> &neighbourhoodData);
    bool runUpdate(int dim, vector<double> &update, Subspace &subspace);
};

#endif
