#include "Cluster.h"

#include <iostream>
#include <set>
#include <vector>


using namespace std;

int Cluster::cnt = 0;

Cluster::Cluster(string name, set<pair<int, int>> &ids, bool noise, int id, vector<double> &mean)
{
    this->name = name;
    this->ids = ids;
    this->id = id;
    this->noise = noise;
    this->mean = mean;
}

int Cluster::size()
{
    return (this->ids).size();
}

string &Cluster::getName()
{
    return this->name;
}

set<pair<int, int>> &Cluster::getIds()
{
    return this->ids;
}

int Cluster::getId()
{
	return this -> id;
}

bool Cluster::isNoise()
{
    return this->noise;
}

void Cluster::setName(string &name)
{
    this->name = name;
}

void Cluster::setIds(set<pair<int, int>> &ids)
{
    this->ids = ids;
}

void Cluster::setNoise(bool noise)
{
    this->noise = noise;
}

void Cluster::setId(int id)
{
	this -> id = id;
}


void Cluster::print()
{
    cout << "name = " << this->name << '\n';
    cout << "ids = ";
    for (auto x : this->ids)
        cout << x.first << ' ';
    cout << '\n';

    cout << "mean = ";
    for (auto x : this->mean)
        cout << x << ' ';
    cout << '\n';
}
