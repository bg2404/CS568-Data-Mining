#ifndef CLUSTER_H
#define CLUSTER_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

class Cluster {
   private:
    string name = "";
    int clusterId;
    map<int, int> ids;
    bool noise = false;
    vector<double> mean;
    int Split;

   public:
    // Cluster Count
    static int cnt;

    // Constructor
    Cluster(string name, map<int, int> &ids, bool noise, int clusterId, vector<double> &mean);
    Cluster(const Cluster &t);
    Cluster();

    // Returns the number of ids inside the cluster
    int size();

    // Getters
    string &getName();
    int getClusterId();
    map<int, int> &getIds();
    bool isNoise();
    vector<int> getIdKeys();
    int getSplit();

    // Setters
    void setName(string &name);
    void setClusterId(int clusterId);
    void setIds(map<int, int> &ids);
    void setNoise(bool noise);
    void setSplit(int split);

    // Displays the Cluster details
    void print();

    //assignment operator
    Cluster &operator=(const Cluster &t);

    //helper functions
    void insertId(int id, int neighCount);
    void deleteId(int id);
    void incrementSplit();
    void resetSplit();
};

#endif
