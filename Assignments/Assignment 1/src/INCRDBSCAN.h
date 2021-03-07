#ifndef INCRDBSCAN_H
#define INCRDBSCAN_H

#include <cstdlib>
#include <map>
#include <vector>

#include "Cluster.h"
#include "Relation.h"
#include "Subspace.h"

#define SUCCESS 0
#define UNCLASSIFIED -1
#define NOISE -2
#define FAILURE -3

using namespace std;

class INCRDBSCAN
{
    Relation<double> m_points;
    vector<double> m_point;
    double m_eps;
    uint m_minPts;
    Subspace m_subspace;
    map<vector<double>, int> m_ids;
    int m_id;

    vector<int> get_neighbourhood(vector<double> p);
    double dist(vector<double>, vector<double>);

    public:
    INCRDBSCAN(vector<double> point, double eps,uint minPts,Relation<double> points,Subspace &subspace, int id);
    Subspace Insert();
};
#endif