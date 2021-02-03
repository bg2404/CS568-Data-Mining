#ifndef SUBCLU_H
#define SUBCLU_H

#include <iostream>
#include <vector>
#include <set>
#include <map>

#include "Cluster.h"
#include "Subspace.h"
#include "Relation.h"
#include "DBSCAN.h"

class SUBCLU {
    protected:
        double epsilon;
        int minPnts;
        int minDim;
        Relation<double> dataBase;

        map<Subspace, vector<Cluster>> Clusterings; //Should we use sets instead of vector?

    public:
        SUBCLU(string filename, int minpnts, double epsilon, int mindim = 1);     
        void run();   
        vector<Cluster> runDBSCAN(Subspace &currSubspace, set<int>& ids = NULL);
        vector<Subspace> generateSubspaceCandidates(vector<Subspace>& subspaces); 
        Subspace besttSubspace(vector<Subspace>& subspaces, Subspace& candidate);
        bool checkLower(Subspace& candidate, vector<Subspace> &subspaces);
        bool contains(vector<Subspace> &subspaces, vector<int> temp);
};


#endif
