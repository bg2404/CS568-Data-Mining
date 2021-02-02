#ifndef DBSCAN_H
#define DBSCAN_H

#include <cstdlib>
#include <vector>

#define SUCCESS 0
#define UNCLASSIFIED -1
#define NOISE -2
#define FAILURE -3

using namespace std;

typedef vector<double> Point;

using Value = std::pair<Point, uint>;

class DBSCAN {
	vector<Point> m_points;
	double m_eps;
	uint m_minPts;
	vector<int> m_clusterIDs;
	vector<vector<Point> > m_clusters;
	uint m_numPoints;
	uint m_subspace;

	int expandCuster(int, uint);
	vector<int> rangeQuery(Point);
	double dist(Point, Point);

	public:
	DBSCAN(vector<Point> points, uint m_subspace, double eps, uint minPts);
	~DBSCAN();
	uint getSubspace();
	double getEps();
	uint getMinPts();
	vector<Point> getPoints();
	uint getNumPoints();
	vector<vector<Point> > getClusters();
};

#endif
