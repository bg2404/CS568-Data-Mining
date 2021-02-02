#include "DBSCAN.h"

#include <cmath>
#include <iostream>
#include <queue>
#include <vector>

DBSCAN::DBSCAN(vector<Point> points, uint subspace, double eps, uint minPts) : m_points(points), m_subspace(subspace), m_eps(eps), m_minPts(minPts) {
	m_numPoints = points.size();
	m_clusterIDs.clear();
	m_clusters.clear();
}
DBSCAN::~DBSCAN() {}

uint DBSCAN::getSubspace() { return m_subspace; }

double DBSCAN::getEps() { return m_eps; }

uint DBSCAN::getMinPts() { return m_minPts; }

vector<Point> DBSCAN::getPoints() { return m_points; }

uint DBSCAN::getNumPoints() { return m_numPoints; }

vector<vector<Point> > DBSCAN::getClusters() {
	if (m_clusters.empty()) {
		int clusterID = 1;
		m_clusterIDs.resize(m_numPoints, UNCLASSIFIED);
		for (uint i = 0; i < m_numPoints; ++i) {
			if (m_clusterIDs[i] == UNCLASSIFIED) {
				if (expandCuster(i, clusterID) == SUCCESS) {
					++clusterID;
				}
			}
		}
		m_clusters.resize(clusterID - 1);
		for (int i = 0; i < m_numPoints; ++i) {
			m_clusters[m_clusterIDs[i] - 1].push_back(m_points[i]);
		}
	}
	return m_clusters;
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
					if (m_clusterIDs[resultP] == UNCLASSIFIED ||
							m_clusterIDs[resultP] == NOISE) {
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
	vector<int> result;
	for (int i = 0; i < m_numPoints; ++i) {
		if (dist(p, m_points[i]) <= m_eps) {
			result.push_back(i);
		}
	}
	return result;
}

double DBSCAN::dist(Point p1, Point p2) {
	double d = 0;
	uint dim = 1;
	for (uint i = 0; i < 32; ++i, dim <<= 1) {
		if (dim & m_subspace) {
			d += ((long long)p1[i] - p2[i]) * (p1[i] - p2[i]);
		}
	}
	return sqrt(d);
}
