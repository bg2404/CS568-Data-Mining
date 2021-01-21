#ifndef DBSCAN_H
#define DBSCAN_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <cstdlib>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

#define SUCCESS 0
#define UNCLASSIFIED -1
#define NOISE -2
#define FAILURE -3

using namespace std;

using Point = bg::model::point<double, 3, bg::cs::cartesian>;

using Value = std::pair<Point, uint>;

class DBSCAN {
    vector<Point> m_points;
    double m_eps;
    uint m_minPts;
    vector<int> m_clusterIDs;
    uint m_numPoints;
    bgi::rtree<Value, bgi::rstar<16>> m_rtree;

   public:
    DBSCAN(vector<Point> points, double eps, uint minPts);
    ~DBSCAN();
    double getEps();
    uint getMinPts();
    vector<Point> getPoints();
    uint getNumPoints();
    vector<int> getClusters();
    int expandCuster(int idx, uint clusterID);
    vector<int> rangeQuery(Point p);
};

#endif
