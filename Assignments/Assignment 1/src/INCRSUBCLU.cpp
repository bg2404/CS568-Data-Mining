#include "INCRSUBCLU.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Cluster.h"
#include "Relation.h"
#include "Subspace.h"
#include "ReadInput.h"
#include "INCRDBSCAN.h"

using namespace std;

// Integer to Binary String
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
        return ret;
    }
}

void INCRSUBCLU::retrieveSubspaces(int n) {
	// Go through all the Subspaces
	//Maintain a set of remaining subspaces
	allSubspaces.clear();
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
		(this -> subspaces).insert(make_pair(dimensions, subspace));
		allSubspaces.insert(dimensions);
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
        (this->dbids).insert(make_pair(dataBase[i],i));
    } 

    retrieveSubspaces(((this -> updates)[0]).size() - 1);

}

void INCRSUBCLU::run() {
	for (vector<double> update : this->updates) {
		// extract update point
		int size = (int)update.size() - 1;
		int type = update[size];
		update.pop_back();

		
		// cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% \n";
		// if(type == -1)
		// 	cout << "Deleting: ";
		// else
		// 	cout << "Inserting: ";
		// for(double x : update)
		// 	cout << x << ' ';
		// cout << '\n';

		// Start Running INCRSUBCLU
		set<vector<int>> remainingSubspaces = allSubspaces;
		if (size <= 1) {
			cout << "Error: INCRSUBCLU needs Multivariate Data";
			return;
		}

		vector<Subspace> candidates;
		// 1-Dimensionality Clustering
		// cout << "Finding clusters in 1-D Subspaces by running INCRDBSCAN....\n";
		for (int dimension = 0; dimension < size; dimension++) {
			Subspace currSubspace(dimension);
			currSubspace = subspaces.find(currSubspace.getDimensions())->second;
			// cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@1\n";

			// currSubspace.print();
			INCRDBSCAN incrdb(update, this -> epsilon, this -> minPnts, this -> dataBase, currSubspace, (this -> dataBase).size(), (this -> dbids));

			if(type == -1)
				currSubspace = incrdb.Delete();
			else
				currSubspace = incrdb.Insert();


			subspaces.find(currSubspace.getDimensions())->second = currSubspace;
			cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
			currSubspace.print();

            //         	cout << "------------------------------\n";
            //         	cout << "Current Subspace: \n";
			// currSubspace.print();
            //         	cout << "------------------------------\n";

			if(incrdb.getChange())
			{
				candidates.push_back(Subspace(currSubspace.getDimensions()));
				remainingSubspaces.erase(currSubspace.getDimensions());
			}
		}

		// cout << "Apriori Buildup starting to identify higher dimension clusters....\n";
		// Apriori BuildUp
		for (int dimensionality = 2; dimensionality <= size; dimensionality++) {
			if (!candidates.empty()) {
				candidates = generateSubspaceCandidates(candidates);

				// cout << "////////////////////////////////////////////////\n";
				// cout << "Candidate Subspaces: \n";
				// for (auto x : candidates) {
				// 	for (auto y : x.getDimensions())
				// 		cout << y;
				// 	cout << ' ';
				// }
				// cout << '\n';
				// cout << "////////////////////////////////////////////////\n";

				vector<Subspace> nextCandidates;	
				for (Subspace candidate : candidates) {
					Subspace currSubspace = subspaces.find(candidate.getDimensions())->second;
					INCRDBSCAN incrdb(update, this -> epsilon, this -> minPnts, this -> dataBase, currSubspace, (this -> dataBase).size(), (this -> dbids));

					if(type == -1)
						currSubspace = incrdb.Delete();
					else
						currSubspace = incrdb.Insert();


					// cout << "------------------------------\n";
					// cout << "Current Subspace: \n";
					// currSubspace.print();
					// cout << "------------------------------\n";

					if(incrdb.getChange())
					{
						nextCandidates.push_back(candidate);
						remainingSubspaces.erase(candidate.getDimensions());
					}

					subspaces.find(currSubspace.getDimensions())->second = currSubspace;
				}
				
				candidates = nextCandidates;
			}
			else
				break;
		}

		//add noise point or simply deleting the point from pruned subspace 
		//TODO find size of epsilon neighbourhood in the cluster or not
		//better way to delete point from the cluster
		for(vector<int> dimensions : remainingSubspaces)
		{
			Subspace subspace = subspaces.find(dimensions)->second;
			if(type == 1)
			{
				int noiseClusterId = subspace.getNoiseClusterId();
				map<int, Cluster>& clusters = subspace.getClusters();
				clusters[noiseClusterId].insertId(dataBase.size(),0);
			}
			else
			{
				map<int, Cluster>& clusters = subspace.getClusters();

				for (auto& cluster : clusters) {
					map<int, int>& ids = cluster.second.getIds();
					if(ids.find(dbids[update]) != ids.end())
					{	clusters[cluster.first].deleteId(dbids[update]);
						break;
					}
				}
			}

			subspaces.find(dimensions)->second = subspace;
		}

		//adding points to database
		if(type==1)
		{
			this->dbids.insert(make_pair(update,this->dataBase.size()));
			this->dataBase.push_back(update);
		}
	}
}

void INCRSUBCLU::print(){

	// printing Subspace*.csv files
    for (pair<vector<int>,Subspace> p : subspaces) {
        string file = "Subspace";
        Subspace subspace = p.second;
        for (auto d : subspace.getDimensions()) {
            file.push_back((char)(d + '0'));
        }
        file = file + ".csv";

        fstream p_file;
        p_file.open(file, ios::out);


        for (pair<int,Cluster> c : subspace.getClusters()) {
			Cluster cluster = c.second;
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


