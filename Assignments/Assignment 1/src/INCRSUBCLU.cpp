#include "INCRSUBCLU.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Cluster.h"
#include "INCRDBSCAN.h"
#include "INCRSUBCLU.h"
#include "ReadInput.h"
#include "Relation.h"
#include "Subspace.h"

using namespace std;

string to_binary(int num) {
    if (num == 0) {
        return "0";
    } else {
        string ret = "";
        while (num) {
            if (num & 1) {
                ret.push_back('1');
            } else {
                ret.push_back('0');
            }
            num >>= 1;
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }
}

INCRSUBCLU::INCRSUBCLU(string databaseFilename, string updatesFilename, int minPnts, double epsilon, int minDim) {
    this->minPnts = minPnts;
    this->epsilon = epsilon;
    this->minDim = minDim;

    ReadInput reader(databaseFilename);
    this->dataBase = reader.read();

    ReadInput reader(updatesFilename);
    this->updates = reader.read();

    for (int i = 0; i < (int)(dataBase.size()); i++) {
        (this->dbids)[dataBase[i]] = i;
    }

    (this->Clusterings).clear();
}

map<Subspace, vector<Cluster>> INCRSUBCLU::run() {
    (this->Clusterings).clear();
    for (auto update : this->updates) {
        int size = (int)update.size() - 1;

        vector<Subspace> subspaces;
        if (size <= 1) {
            cout << "Error: INCRSUBCLU needs Multivariate Data";
            return (this->Clusterings);
        }
        // 1-Dimensionality Clustering
        cout << "Finding clusters in 1-D Subspaces by running INCRDBSCAN....\n";
        for (int dimension = 0; dimension < size; dimension++) {
            Subspace currSubspace(dimension);
            cout << "------------------------------\n";
            cout << "Current Subspace: \n";
            currSubspace.print();
            // ReadInput reader("Subspace" + to_binary(dimension) + ".csv");
            // auto clusterData = reader.read();
            // INCRDBSCAN incrdbscan();
            // DBSCAN dbscan(this->dataBase, currSubspace, this->epsilon, this->minPnts, this->dbids);

            // vector<Cluster> clusters = dbscan.getClusters();
            if (runUpdate(dimension, update, currSubspace)) {
                vector<Cluster> clusters;
                map<int, Cluster> clusterMap;
                for (auto cluster : clusterMap) {
                    clusters.push_back(cluster.second);
                }
                (this->Clusterings).emplace(currSubspace, clusters);
                subspaces.push_back(currSubspace);
                currSubspace.setClusters(clusters);
                cout << "Clusters Present\n";
            }
            // cout << "Number of Clusters: " << clusters.size() << '\n';

            cout << "------------------------------\n";
        }

        cout << "Apriori Buildup starting to identify higher dimension clusters....\n";
        // Apriori BuildUp
        for (int dimensionality = 2; dimensionality <= size; dimensionality++) {
            if (!subspaces.empty()) {
                vector<Subspace> candidates = generateSubspaceCandidates(subspaces);
                vector<Subspace> sub_d;

                for (Subspace candidate : candidates) {
                    cout << "------------------------------\n";
                    cout << "Current Subspace: \n";
                    candidate.print();
                    Subspace bestSubspace = besttSubspace(subspaces, candidate);

                    if (!bestSubspace.isValid()) {
                        cout << "BestSubspace return empty subspace\n";
                    }

                    // TODO: Yha dekho kya krna h?
                    // TODO: BEGIN
                    vector<Cluster> clusters;
                    if (runUpdate(dimensionality, update, candidate)) {
                        map<int, Cluster> clusterMap;
                        for (auto cluster : clusterMap) {
                            clusters.push_back(cluster.second);
                        }
                        if (clusters.size() > 1) {
                            sub_d.push_back(candidate);
                            (this->Clusterings).emplace(candidate, clusters);
                            cout << "Clusters Present\n";
                        }
                    }

                    // if ((this->Clusterings).find(bestSubspace) != (this->Clusterings).end()) {
                    //     for (auto cluster : (((this->Clusterings).find(bestSubspace))->second)) {
                    //         if (cluster.size() >= this->minPnts) {
                    //             map<int, int> idPairs = cluster.getIds();
                    //             set<int> ids;
                    //             for (auto x : idPairs)
                    //                 ids.insert(x.first);
                    //             vector<Cluster> candidateClusters = runUpdate(dimensionality, update, bestSubspace) runDBSCAN(candidate, ids);

                    //             if (!(candidateClusters.size() == 1 && candidateClusters.front().isNoise())) {
                    //                 for (auto currCluster : candidateClusters)
                    //                     clusters.push_back(currCluster);
                    //             }
                    //         }
                    //     }

                    // } else
                    //     cout << "Map error again\n";

                    // if (!clusters.empty()) {
                    //     sub_d.push_back(candidate);
                    //     (this->Clusterings).insert({candidate, clusters});
                    //     candidate.setClusters(clusters);
                    //     cout << "Clusters Present\n";
                    // }
                    // TODO: END
                    cout << "Number of Clusters: " << clusters.size() << '\n';
                    cout << "------------------------------\n";
                }

                subspaces = sub_d;
            } else
                break;
        }

        return (this->Clusterings);
    }
}

vector<Subspace> INCRSUBCLU::generateSubspaceCandidates(vector<Subspace> &subspaces) {
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

    cout << "////////////////////////////////////////////////\n";
    cout << "Candidate Subspaces: \n";
    for (auto x : candidates) {
        for (auto y : x.getDimensions())
            cout << y;
        cout << ' ';
    }
    cout << '\n';

    cout << "////////////////////////////////////////////////\n";
    return vector<Subspace>(candidates.begin(), candidates.end());
}

bool INCRSUBCLU::checkLower(Subspace &candidate, vector<Subspace> &subspaces) {
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

Subspace INCRSUBCLU::besttSubspace(vector<Subspace> &subspaces, Subspace &candidate) {
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
        cout << "Best Subspace: ";
        for (auto x : theBest.getDimensions())
            cout << x;
        cout << '\n';

        return theBest;
    } else
        return Subspace();
}

// vector<Cluster> INCRSUBCLU::runDBSCAN(Subspace &currSubspace, set<int> &ids) {
//     Relation<double> points;
//     for (auto i : ids)
//         points.push_back(dataBase[i]);
//     DBSCAN dbscan(points, currSubspace, this->epsilon, this->minPnts, this->dbids);
//     return dbscan.getClusters();
// }

int INCRSUBCLU::getSubspaceData(int dim, Relation<double> &database, vector<Cluster> &clusterData, Subspace &subspace, map<vector<double>, int> &neighbourhoodData) {
    string subspaceFilename = "Subspace" + to_binary(dim) + ".csv";
    ifstream fin(subspaceFilename);
    if (fin.is_open()) {
        fin.close();
        ReadInput reader(subspaceFilename);
        Relation<double> subspaceClusterData = reader.read();
        int i = 0;
        for (vector<double> point : subspaceClusterData) {
            if (point.back() >= clusterData.size()) {
                clusterData.resize(point.back() + 1);
            }
            int label = point.back();
            point.pop_back();
            int neighbour_count = point.back();
            point.pop_back();
            neighbourhoodData.emplace(point, neighbour_count);
            clusterData[label].insertId(i, neighbour_count);
            database.push_back(point);
            ++i;
        }
        for (int i = 0; i < clusterData.size(); ++i) {
            string name = "Cluster_" + to_string(i);
            clusterData[i].setName(name);
            clusterData[i].setClusterId(i);
        }
        subspace.setClusters(clusterData);
        return 1;
    }
    return 0;
}

void saveSubspaceData(Subspace &subspace, int dim) {
    // TODO: Write a subspace to a file
    // string subspaceFilename = "Subspace" + to_binary(dim) + ".csv";
    // ofstream fout(subspaceFilename);
    // if (fout.is_open()) {
    //     map<int, Cluster> clusters = subspace.getClusters();
    //     clusters[0].
    // }
}

bool INCRSUBCLU::runUpdate(int dim, vector<double> &update, Subspace &subspace) {
    Relation<double> database;
    vector<Cluster> clusterData;
    map<vector<double>, int> neighbourhoodData;
    if (getSubspaceData(dim, database, clusterData, subspace, neighbourhoodData)) {
        if (update.back() > 0) {  // Insert
            update.pop_back();
            INCRDBSCAN incrdbscan(update, this->epsilon, this->minPnts, database, subspace, database.size(), neighbourhoodData);
            subspace = incrdbscan.Insert();
        } else {  // Delete
            update.pop_back();
            int id = -1, i = 0;
            for (auto point : database) {
                if (point == update) {
                    id = i;
                    break;
                }
                ++i;
            }
            if (id < 0) {
                return;
            }
            INCRDBSCAN incrdbscan(update, this->epsilon, this->minPnts, database, subspace, id, neighbourhoodData);
            subspace = incrdbscan.Delete();
        }
    } else {
        // TODO: Subspace_dim.csv not found
    }
    // TODO: return changeInCluster ? True : False
    return true;
}