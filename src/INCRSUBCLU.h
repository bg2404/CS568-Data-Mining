#ifndef INCRSUBCLU_H
#define INCRSUBCLU_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

#include "Cluster.h"
#include "Relation.h"
#include "Subspace.h"
#include "ReadInput.h"
#include "INCRDBSCAN.h"

class INCRSUBCLU {
   protected:
    double epsilon;
    int minPnts;
    int minDim;
    Relation<double> dataBase;
    Relation<double> updates;
    set<vector<int>> allSubspaces;
    //subspace map
    map<vector<int>, Subspace> subspaces;

    // map instances to index
    map<vector<double>, int> dbids;

   public:
    INCRSUBCLU(string databaseFilename, string updatesFilename, int minpnts, double epsilon, int mindim = 1);
    void retrieveSubspaces(int n);
    void run();
    vector<Subspace> generateSubspaceCandidates(vector<Subspace> &subspaces);
    bool checkLower(Subspace &candidate, vector<Subspace> &subspaces);
    void print();
};

#endif
