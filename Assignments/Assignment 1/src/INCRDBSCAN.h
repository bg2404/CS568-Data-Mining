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

struct point 
{
	int id, neighCount, clusterId;

	point(int id, int neighCount, int clusterId)
	{
		this -> id = id;
		this -> neighCount = neighCount;
		this -> clusterId = clusterId;
	}
    bool operator<(const point& t) const
    { 
        return (this->id < t.id); 
    } 
};

class INCRDBSCAN
{
    Relation<double> m_points;
    vector<double> m_point;
    double m_eps;
    uint m_minPts;
    Subspace m_subspace;
    map<vector<double>, int> m_ids;
    int m_id;

    vector<point> getAndIncrementNeighbourhood();
    vector<point> getAndDecrementNeighbourhood(int *pCid);
    vector<point> getPointsLostCore(vector<point> epsilonNeighbourhood);
    int getNearestCorePointClusterId(point p);
    void addPointsToCheck(point p,set<point> &pointsToCheck);
    double dist(vector<double>&, vector<double>&);

    public:
    INCRDBSCAN(vector<double> point, double eps,uint minPts,Relation<double> points,Subspace &subspace, int id,map<vector<double>,int> ids);
    Subspace Insert();
    Subspace Delete();
};
#endif
