#include "Subspace.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#include "BitsUtil.h"
#include "Cluster.h"

using namespace std;

Subspace::Subspace(int dimension) {
    (this->dimensions).assign(dimension + 1, 0);
    (this->dimensions)[dimension] = 1;
    this->dimensionality = 1;
}

Subspace::Subspace() {
    this->dimensions = vector<int>();
    this->dimensionality = 0;
}

Subspace::Subspace(vector<int> &dimensions) {
    this->dimensions = dimensions;
    this->dimensionality = count(dimensions.begin(), dimensions.end(), 1);
}

vector<int> &Subspace::getDimensions() {
    return this->dimensions;
}

int &Subspace::getDimensionality() {
    return this->dimensionality;
}

map<int, Cluster> &Subspace::getClusters() {
    return (this->clusters);
}

void Subspace::addDimension(int dimension) {
    if (dimension < 0) {
        cout << "addDimension invalid argument < 0\n";
        return;
    }
    int size = (this->dimensions).size();

    if (dimension < size) {
        if ((this->dimensions)[dimension] == 0)
            (this->dimensionality)++;

        (this->dimensions)[dimension] = 1;
    } else if (dimension == size) {
        (this->dimensionality)++;
        (this->dimensions).push_back(1);
    } else {
        cout << "addDimension invalid argument > size\n";
    }
}

void Subspace::removeDimension(int dimension) {
    if (dimension < 0) {
        cout << "removeDimension invalid argument < 0\n";
        return;
    }
    int size = (this->dimensions).size();

    if (dimension < size) {
        if ((this->dimensions)[dimension] == 1)
            (this->dimensionality)--;

        if (dimension < size - 1)
            (this->dimensions)[dimension] = 0;
        else
            (this->dimensions).pop_back();
    } else
        cout << "removeDimension invalid argument >= size\n";
}

Subspace Subspace::join(Subspace &other) {
    vector<int> newDimensions = joinLastDimensions(other);
    return Subspace(newDimensions);
}

vector<int> Subspace::joinLastDimensions(Subspace &other) {
    vector<int> result;
    if ((this->dimensionality) != other.dimensionality)
        return vector<int>();

    if ((this->dimensions).size() >= (other.dimensions).size())
        return vector<int>();

    if ((this->dimensionality) == 1)
        return BitsUtil::orVectors(this->dimensions, other.dimensions);
    result = BitsUtil::orVectors(this->dimensions, other.dimensions);

    if (count(result.begin(), result.end(), 1) != this->dimensionality + 1)
        return vector<int>();

    return result;
}

bool Subspace::isSubspace(Subspace &subspace) {
    if ((this->dimensionality) <= subspace.dimensionality) {
        if ((this->dimensionality) == BitsUtil::intersection(this->dimensions, subspace.dimensions))
            return true;
        return false;
    }
    return false;
}

bool Subspace::hasDimension(int i) {
    if (i < (int)((this->dimensions).size())) {
        return (this->dimensions)[i];
    }
    return false;
}

void Subspace::print() {
    cout << "dimensionality = " << this->dimensionality << '\n';
    cout << "dimensions = ";
    for (auto x : this->dimensions)
        cout << x;
    cout << '\n';

    for (auto it : clusters) {
        cout << "Cluster_" << it.first << ": ";
        Cluster cluster = it.second;

	if(cluster.isNoise())
		cout << "(Noise)\n";
	else
		cout << "\n";

	cout << "ids: ";
        map<int, int> ids = cluster.getIds();
        for (pair<int, int> point : ids) {
            cout << point.first << " "; 
        }
	cout << '\n';
    }
}

bool Subspace::isValid() {
    return ((this->dimensionality) != 0 && !(this->dimensions).empty());
}

bool Subspace::operator<(const Subspace &s2) const {
    if ((this->dimensionality) < s2.dimensionality)
        return true;
    else if ((this->dimensionality) > s2.dimensionality)
        return false;
    else if ((this->dimensions).size() < s2.dimensions.size())
        return true;
    else if ((this->dimensions).size() > s2.dimensions.size())
        return false;
    else {
        for (int i = (int)((this->dimensions).size()) - 1; i >= 0; i--) {
            if ((dimensions[i] + s2.dimensions[i]) == 1) {
                if (dimensions[i] == 1)
                    return false;
                else
                    return true;
            }
        }
    }
    return false;
}

Subspace &Subspace::operator=(const Subspace &s2)
{
    this->dimensions = s2.dimensions;
    this->dimensionality = s2.dimensionality;
    this->clusters = s2.clusters;
    return *this;
}

void Subspace::setClusters(vector<Cluster> &clustering) {
    map<int, Cluster> clusters;

    for (auto cluster : clustering) {
        int clusterId = cluster.getClusterId();
        clusters.insert(make_pair(clusterId, cluster));
    }
    (this->clusters) = clusters;
}

void Subspace::setClusters(map<int, Cluster> &clusters) {
    (this->clusters) = clusters;
}

void Subspace::insertCluster(Cluster cluster) {
    (this->clusters).insert(make_pair(cluster.getClusterId(), cluster));
}

void Subspace::deleteCluster(int clusterId) {
    (this->clusters).erase(clusterId);
}

int Subspace::getNoiseClusterId() {
    for (auto cluster : (this->clusters)) {
        if (cluster.second.isNoise())
            return cluster.first;
    }
    return -1;
}
