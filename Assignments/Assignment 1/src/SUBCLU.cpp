#include "SUBCLU.h"

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>
#include <string>

#include "Cluster.h"
#include "Subspace.h"
#include "Relation.h"
#include "DBSCAN.h"
#include "ReadInput.h"

using namespace std;

SUBCLU::SUBCLU(string filename, int minPts, double epsilon, int minDim)
{
    this->minPnts = minPnts;
    this->epsilon = epsilon;
    this->minDim = minDim;

    ReadInput reader(filename);
    this->dataBase = reader.read();

    for (int i = 0; i < dataBase.size(); i++)
    {
        this->dbids[dataBase[i]] = i;
    }
}

map<Subspace, vector<Cluster>> SUBCLU::run()
{
    int dimensionality = dataBase[0].size();
    vector<Subspace> subspaces;
    if (dimensionality <= 1)
    {
        cout << "Error: SUBCLU needs Multivariate Data";
        return Clusterings;
    }

    for (int dim = 0; dim < dimensionality; dim++)
    {
        Subspace currSubspace(dim);
        DBSCAN dbscan(dataBase, currSubspace, epsilon, minPnts, dbids);
        vector<Cluster> clusters = dbscan.getClusters();

        if (!clusters.empty())
        {
            subspaces.push_back(currSubspace);
            Clusterings[currSubspace] = clusters;
        }
    }

    for (int dim = 2; dim <= dimensionality; dim++)
    {
        vector<Subspace> candidates = generateSubspaceCandidates(subspaces);
        vector<Subspace> sub_d;

        for (Subspace candidate : candidates)
        {
            Subspace bestSubspace = besttSubspace(subspaces, candidate);
            vector<Cluster> clusters;

            for (auto cluster : Clusterings[bestSubspace])
            {
                if (cluster.size() >= this->minPnts)
                {
                    vector<Cluster> candidateClusters = runDBSCAN(candidate, cluster.getIds());
                    if (!candidateClusters.empty())
                    {
                        for (auto currCluster : candidateClusters)
                            clusters.push_back(currCluster);
                    }
                }
            }

            if (!clusters.empty())
            {
                sub_d.push_back(candidate);
                Clusterings[candidate] = clusters;
            }
        }

        subspaces = sub_d;
    }

    int numClusters = 0;
    set<int> noise;

    map<Subspace, vector<Cluster>> filtered;
    for (auto it = Clusterings.rbegin(); it != Clusterings.rend(); it++)
    {
        Subspace subspace = it->first;
        if (subspace.getDimensionality() < minDim)
            continue;
    }

    return Clusterings;
}

vector<Subspace> SUBCLU::generateSubspaceCandidates(vector<Subspace> &subspaces)
{
    if (subspaces.empty())
        return vector<Subspace>();
    vector<Subspace> candidates;
    int dim = subspaces[0].getDimensionality() + 1;

    for (int i = 0; i < subspaces.size(); i++)
    {
        Subspace s1 = subspaces[i];
        for (int j = 0; j < subspaces.size(); j++)
        {
            Subspace candidate = s1.join(subspaces[j]);

            if (!candidate.isValid())
                continue;

            if (dim == 2 || checkLower(candidate, subspaces))
                candidates.push_back(candidate);
        }
    }

    return candidates;
}

bool SUBCLU::checkLower(Subspace &candidate, vector<Subspace> &subspaces)
{
    vector<int> dimensions = candidate.getDimensions();
    for (int dim = 0; dim < dimensions.size(); dim++)
    {
        if (dimensions[dim] == 1)
        {
            vector<int> temp = dimensions;
            temp[dim] == 0;
            if (!contains(subspaces, temp))
                return false;
        }
    }
    return true;
}

bool SUBCLU::contains(vector<Subspace> &subspaces, vector<int> temp)
{
    for (Subspace subspace : subspaces)
    {
        vector<int> comp = subspace.getDimensions();
        bool flag = 1;
        for (int i = 0; i < comp.size(); i++)
        {
            if (i >= temp.size() && comp[i] == 1)
                flag = 0;
            else if (comp[i] != temp[i])
                flag = 0;
        }
        if (flag)
            return true;
    }
    return false;
}

Subspace SUBCLU::besttSubspace(vector<Subspace> &subspaces, Subspace &candidate)
{
    Subspace theBest;
    long min = (long)INT64_MAX;
    for (Subspace subspace : subspaces)
    {
        if (subspace.isSubspace(candidate))
        {
            int sum = 0;
            for (Cluster cluster : Clusterings[subspace])
                sum += cluster.size();
            if (sum < min)
            {
                min = sum;
                theBest = subspace;
            }
        }
    }

    return theBest;
}

vector<Cluster> SUBCLU::runDBSCAN(Subspace &currSubspace, set<int> &ids)
{
    Relation<double> points;
    for (auto i : ids)
    {
        points.push_back(dataBase[i]);
    }
    DBSCAN dbscan(points, currSubspace, epsilon, minPnts, dbids);
    return dbscan.getClusters();
}