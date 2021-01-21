#include "dbscan.h"

#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <iostream>
#include <queue>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

DBSCAN::DBSCAN(vector<Point> points, double eps, uint minPts) : m_points(points), m_eps(eps), m_minPts(minPts) {
    m_numPoints = points.size();
    m_clusterIDs.clear();
    m_rtree.insert(points | boost::adaptors::indexed() | boost::adaptors::transformed([](const auto& iv) {
                       return std::make_pair(iv.value(), iv.index());
                   }));
}
DBSCAN::~DBSCAN() {
}

double DBSCAN::getEps() {
    return m_eps;
}

uint DBSCAN::getMinPts() {
    return m_minPts;
}

vector<Point> DBSCAN::getPoints() {
    return m_points;
}

uint DBSCAN::getNumPoints() {
    return m_numPoints;
}

vector<int> DBSCAN::getClusters() {
    m_clusterIDs.resize(m_numPoints, UNCLASSIFIED);
    int clusterID = 1;
    for (uint i = 0; i < m_numPoints; ++i) {
        if (m_clusterIDs[i] == UNCLASSIFIED) {
            if (expandCuster(i, clusterID) == SUCCESS) {
                ++clusterID;
            }
        }
    }
    return m_clusterIDs;
}

int DBSCAN::expandCuster(int idx, uint clusterID) {
    vector<int> seeds = rangeQuery(m_points[idx]);
    if (seeds.size() < m_minPts) {
        m_clusterIDs[idx] = NOISE;
        return FAILURE;
    } else {
        queue<int> qseeds;
        for (uint i = 0; i < seeds.size(); ++i) {
            m_clusterIDs[seeds[i]] = clusterID;
            if (seeds[i] != idx) {
                qseeds.push(seeds[i]);
            }
        }

        while (!qseeds.empty()) {
            int currentP = qseeds.front();
            vector<int> result = rangeQuery(m_points[currentP]);
            if (result.size() >= m_minPts) {
                for (int resultP : result) {
                    if (m_clusterIDs[resultP] == UNCLASSIFIED || m_clusterIDs[resultP] == NOISE) {
                        if (m_clusterIDs[resultP] == UNCLASSIFIED) {
                            qseeds.push(resultP);
                        }
                        m_clusterIDs[resultP] = clusterID;
                    }
                }
            }
            qseeds.pop();
        }
        return SUCCESS;
    }
}

vector<int> DBSCAN::rangeQuery(Point p) {
    double x = bg::get<0>(p);
    double y = bg::get<1>(p);
    double z = bg::get<2>(p);
    bg::model::box<Point> query_box(Point(x - m_eps, y - m_eps, z - m_eps), Point(x + m_eps, y + m_eps, z + m_eps));
    vector<int> result;
    for (auto it = m_rtree.qbegin(bgi::within(query_box)); it != m_rtree.qend(); ++it) {
        if (bg::distance(p, it->first) <= m_eps) {
            result.push_back(it->second);
        }
    }
    return result;
}