#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <set>
#include <cstdlib>
#include <vector>
using namespace std;

class Cluster
{
private:
    string name = "";
    int id;
    set<pair<int,int>> ids;
    bool noise = false;
    vector<double> mean;

public:
    // Cluster Count
    static int cnt;

    // Constructor
    Cluster(string name, set<pair<int, int>> &ids, bool noise, int id, vector<double> &mean);
    Cluster();

    // Returns the number of ids inside the cluster
    int size();

    // Getters
    string &getName();
    int getId();
    set<pair<int, int>> &getIds();
    bool isNoise();

    // Setters
    void setName(string &name);
    void setId(int id);
    void setIds(set<pair<int, int>> &ids);
    void setNoise(bool noise);

    // Displays the Cluster details
    void print();

    //assignment operator
    Cluster & operator = (const Cluster &t); 
    // copy constructor
    Cluster(const Cluster &t);
};

#endif
