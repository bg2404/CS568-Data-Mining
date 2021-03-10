#ifndef DBSCAN_H
#define DBSCAN_H

#include <cstdlib>
#include <map>
#include <vector>

#include "Cluster.h"
#include "Relation.h"
#include "Subspace.h"
#include "DBSCAN.h"

#define SUCCESS 0
#define UNCLASSIFIED -1
#define NOISE -2
#define FAILURE -3

using namespace std;

using Value = std::pair<vector<double>, uint>;

class DBSCAN {
    Relation<double> m_points;
    double m_eps;
    uint m_minPts;
    vector<int> m_clusterIDs;
    vector<int> m_neighCounts;
    vector<Cluster> m_clusters;
    uint m_numPoints;
    Subspace m_subspace;
    map<vector<double>, int> m_ids;

    int expandCuster(int, uint);
    vector<int> rangeQuery(vector<double>);
    double dist(vector<double>, vector<double>);
    vector<double> getMean(vector<int> &v);

   public:
    DBSCAN(Relation<double> points, Subspace m_subspace, double eps, uint minPts, map<vector<double>, int> ids);
    ~DBSCAN();
    Subspace getSubspace();
    double getEps();
    uint getMinPts();
    Relation<double> getPoints();
    uint getNumPoints();
    vector<Cluster> getClusters();
};

#endif
