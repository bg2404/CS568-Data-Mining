#ifndef SUBCLU_H
#define SUBCLU_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

#include "Cluster.h"
#include "DBSCAN.h"
#include "Relation.h"
#include "ReadInput.h"
#include "Subspace.h"

class SUBCLU {
   protected:
    double epsilon;
    int minPnts;
    int minDim;
    Relation<double> dataBase;

    // map subspaces to their respective clusters
    map<Subspace, vector<Cluster>> Clusterings;

    // map instances to index
    map<vector<double>, int> dbids;

   public:
    SUBCLU(string filename, int minpnts, double epsilon, int mindim = 1);
    map<Subspace, vector<Cluster>> run();
    vector<Cluster> runDBSCAN(Subspace &currSubspace, set<int> &ids);
    vector<Subspace> generateSubspaceCandidates(vector<Subspace> &subspaces);
    Subspace besttSubspace(vector<Subspace> &subspaces, Subspace &candidate);
    bool checkLower(Subspace &candidate, vector<Subspace> &subspaces);
};

#endif
