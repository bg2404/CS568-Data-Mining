#include "SUBCLU.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Cluster.h"
#include "DBSCAN.h"
#include "ReadInput.h"
#include "Relation.h"
#include "Subspace.h"

using namespace std;

SUBCLU::SUBCLU(string filename, int minPnts, double epsilon, int minDim) {
    this->minPnts = minPnts;
    this->epsilon = epsilon;
    this->minDim = minDim;

    ReadInput reader(filename);
    this->dataBase = reader.read();

    for (int i = 0; i < (int)(dataBase.size()); i++) {
        (this->dbids)[dataBase[i]] = i;
    }

    (this->Clusterings).clear();
}

map<Subspace, vector<Cluster>> SUBCLU::run() {
    (this->Clusterings).clear();
    if ((this->dataBase).size()) {
        int size = (this->dataBase)[0].size();

        vector<Subspace> subspaces;
        if (size <= 1) {
            cout << "Error: SUBCLU needs Multivariate Data";
            return (this->Clusterings);
        }
        // 1-Dimensionality Clustering
        // cout << "Finding clusters in 1-D Subspaces by running DBSCAN....\n";
        for (int dimension = 0; dimension < size; dimension++) {
            Subspace currSubspace(dimension);
            // cout << "------------------------------\n";
            // cout << "Current Subspace: \n";
            // currSubspace.print();
            DBSCAN dbscan(this->dataBase, currSubspace, this->epsilon, this->minPnts, this->dbids);

            vector<Cluster> clusters = dbscan.getClusters();
            if (!(clusters.size() == 1 && clusters.back().isNoise())) {
                (this->Clusterings).insert({currSubspace, clusters});
                subspaces.push_back(currSubspace);
                currSubspace.setClusters(clusters);
                // cout << "Clusters Present\n";
            }
            // cout << "Number of Clusters: " << clusters.size() << '\n';

            // cout << "------------------------------\n";
        }

        // cout << "Apriori Buildup starting to identify higher dimension clusters....\n";
        // Apriori BuildUp
        for (int dimensionality = 2; dimensionality <= size; dimensionality++) {
            if (!subspaces.empty()) {
                vector<Subspace> candidates = generateSubspaceCandidates(subspaces);
                vector<Subspace> sub_d;

                for (Subspace candidate : candidates) {
                    // cout << "------------------------------\n";
                    // cout << "Current Subspace: \n";
                    // candidate.print();
                    Subspace bestSubspace = besttSubspace(subspaces, candidate);

                    if (!bestSubspace.isValid()) {
                        // cout << "BestSubspace return empty subspace\n";
                    }

                    vector<Cluster> clusters;

                    if ((this->Clusterings).find(bestSubspace) != (this->Clusterings).end()) {
                        for (auto cluster : (((this->Clusterings).find(bestSubspace))->second)) {
                            if (cluster.size() >= this->minPnts) {
                                map<int, int> idPairs = cluster.getIds();
                                set<int> ids;
                                for (auto x : idPairs)
                                    ids.insert(x.first);
                                vector<Cluster> candidateClusters = runDBSCAN(candidate, ids);

                                if (!(candidateClusters.size() == 1 && candidateClusters.back().isNoise())) {
                                    for (auto currCluster : candidateClusters)
                                        clusters.push_back(currCluster);
                                }
                            }
                        }

                    } else
                        cout << "Map error again\n";

                    if (!clusters.empty()) {
                        sub_d.push_back(candidate);
                        (this->Clusterings).insert({candidate, clusters});
                        candidate.setClusters(clusters);
                        // cout << "Clusters Present\n";
                    }
                    // cout << "Number of Clusters: " << clusters.size() << '\n';
                    // cout << "------------------------------\n";
                }

                subspaces = sub_d;
            } else
                break;
        }

        return (this->Clusterings);
    } else {
        cout << "Empty Dataset Nothing to Cluster\n";
        return (this->Clusterings);
    }
}

vector<Subspace> SUBCLU::generateSubspaceCandidates(vector<Subspace> &subspaces) {
    if (subspaces.empty())
        return vector<Subspace>();

    set<Subspace> candidates;
    int dimensionality = subspaces[0].getDimensionality() + 1;

    for (int i = 0; i < (int)(subspaces.size()); i++) {
        for (int j = 0; j < (int)(subspaces.size()); j++) {
            if (i != j) {
                Subspace candidate = subspaces[i].join(subspaces[j]);

                if (!candidate.isValid())
                    continue;

                if (dimensionality == 2 || checkLower(candidate, subspaces))
                    candidates.insert(candidate);
            }
        }
    }

    // cout << "////////////////////////////////////////////////\n";
    // cout << "Candidate Subspaces: \n";
    // for (auto x : candidates) {
    //     for (auto y : x.getDimensions())
    //         cout << y;
    //     cout << ' ';
    // }
    // cout << '\n';

    // cout << "////////////////////////////////////////////////\n";
    return vector<Subspace>(candidates.begin(), candidates.end());
}

bool SUBCLU::checkLower(Subspace &candidate, vector<Subspace> &subspaces) {
    int size = candidate.getDimensions().size();

    Subspace candidateCopy = candidate;

    for (int dimension = 0; dimension < size; dimension++) {
        if (candidate.hasDimension(dimension)) {
            candidateCopy.removeDimension(dimension);
            if (!candidateCopy.isSubspace(candidate))
                return false;
            candidateCopy.addDimension(dimension);
        }
    }
    return true;
}

Subspace SUBCLU::besttSubspace(vector<Subspace> &subspaces, Subspace &candidate) {
    if (subspaces.size()) {
        Subspace theBest = subspaces[0];
        long min = (long)INT64_MAX;
        for (Subspace subspace : subspaces) {
            if (subspace.isSubspace(candidate)) {
                int sum = 0;
                for (Cluster cluster : ((this->Clusterings).find(subspace)->second))
                    sum = sum + cluster.size();
                if (sum < min) {
                    min = sum;
                    theBest = subspace;
                }
            }
        }
        // cout << "Best Subspace: ";
        // for (auto x : theBest.getDimensions())
        //     cout << x;
        // cout << '\n';

        return theBest;
    } else
        return Subspace();
}

vector<Cluster> SUBCLU::runDBSCAN(Subspace &currSubspace, set<int> &ids) {
    Relation<double> points;
    for (auto i : ids)
        points.push_back(dataBase[i]);
    DBSCAN dbscan(points, currSubspace, this->epsilon, this->minPnts, this->dbids);
    return dbscan.getClusters();
}
