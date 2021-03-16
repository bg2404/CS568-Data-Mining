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

void INCRSUBCLU::retrieveSubspaces(int n) {
	// Go through all the Subspaces
	for(long long i = 1; i < (1 << n); i++) {

		// Create Subspace Name
		string file = "Subspace";
		string d = to_binary(i);
		vector<int> dimensions(d.length());
		for(int i = 0; i < (int)dimensions.size(); i++)
			dimensions[i] = d[i] - '0';
		file = file + d + ".csv";

		//Read Subspace File
		ReadInput reader(file);
		Subspace subspace = reader.readSubspace(dimensions);
		(this -> subspaces).insert({dimensions, subspace});
	}
}

INCRSUBCLU::INCRSUBCLU(string databaseFilename, string updatesFilename, int minPnts, double epsilon, int minDim) {
    this->minPnts = minPnts;
    this->epsilon = epsilon;
    this->minDim = minDim;
    this->subspaces.clear();

    //file reading
    ReadInput reader(databaseFilename);
    this->dataBase = reader.read();

    ReadInput reader_2(updatesFilename);
    this->updates = reader_2.read();

    for (int i = 0; i < (int)(dataBase.size()); i++) {
        (this->dbids)[dataBase[i]] = i;
    } 

    retrieveSubspaces(((this -> updates)[0]).size() - 1);

}

void INCRSUBCLU::run() {
	for (vector<double> update : this->updates) {

		int size = (int)update.size() - 1;
		int type = update[size];
		update.pop_back();


		if (size <= 1) {
			cout << "Error: INCRSUBCLU needs Multivariate Data";
			return;
		}

		vector<Subspace> candidates;
		// 1-Dimensionality Clustering
		cout << "Finding clusters in 1-D Subspaces by running INCRDBSCAN....\n";
		for (int dimension = 0; dimension < size; dimension++) {
			Subspace currSubspace(dimension);
			currSubspace = subspaces[(currSubspace.getDimensions())];

			INCRDBSCAN incrdb(update, this -> epsilon, this -> minPnts, this -> dataBase, currSubspace, (this -> dataBase).size(), (this -> dbids));

			if(type == -1)
				currSubspace = incrdb.Delete();
			else
				currSubspace = incrdb.Insert();

			subspaces[(currSubspace.getDimensions())] = currSubspace;

			candidates.push_back(Subspace(currSubspace.getDimensions()));
		}

		cout << "Apriori Buildup starting to identify higher dimension clusters....\n";
		// Apriori BuildUp
		for (int dimensionality = 2; dimensionality <= size; dimensionality++) {
			if (!candidates.empty()) {
				vector<Subspace> nextCandidates = generateSubspaceCandidates(candidates);

				for (Subspace candidate : nextCandidates) {
					Subspace currSubspace = subspaces[candidate.getDimensions()];
					INCRDBSCAN incrdb(update, this -> epsilon, this -> minPnts, this -> dataBase, currSubspace, (this -> dataBase).size(), (this -> dbids));

					if(type == -1)
						currSubspace = incrdb.Delete();
					else
						currSubspace = incrdb.Insert();

					subspaces[(currSubspace.getDimensions())] = currSubspace;
					//TODO: prune subspace with no change


				}
				
				candidates = nextCandidates;

			}
			else
				break;
		}

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


