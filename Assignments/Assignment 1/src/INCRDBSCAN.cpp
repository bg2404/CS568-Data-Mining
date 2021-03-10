#include "INCRDBSCAN.h"

#include <assert.h>

#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

#include "Cluster.h"
#include "Relation.h"
#include "Subspace.h"
#include "DBSCAN.h"

using namespace std;


INCRDBSCAN::INCRDBSCAN(vector<double> point, double eps, uint minPts, Relation<double> points ,Subspace &subspace, int id, map<vector<double>,int> ids)
{
	m_point = point;
	m_eps = eps;
	m_minPts = minPts;
	m_points = points;
	m_subspace = subspace;
	m_id = id;
	m_ids = ids;
}

Subspace INCRDBSCAN::Insert()
{
	// useful declarations
	bool core_p = false;   
	bool has_core = false;
	int noiseClusterId = m_subspace.getNoiseClusterId();
	map<int,Cluster>& clusters = m_subspace.getClusters();
	
	// find epsilon neighbourhood 
	vector<point> epsilon_neighbourhood = getAndIncrementNeighbourhood();
	
	// find if there exist a core point or not
	core_p = ((uint)epsilon_neighbourhood.size() >= m_minPts);
	has_core = core_p;
	for(auto neighPoint : epsilon_neighbourhood)
		has_core |= (neighPoint.neighCount >= (int)m_minPts);

	//case1 
	if(!has_core)
	{
		// add point to noise
		clusters[noiseClusterId].insertId(m_id, epsilon_neighbourhood.size());
	}
	//case2
	else 
	{
		int noise_core = 0;
		int cluster_core = 0;
		for(point p: epsilon_neighbourhood)
		{
			if(p.neighCount >= (int)m_minPts)
			{
				if(p.clusterId == noiseClusterId)
					noise_core++;
				else
					cluster_core++;
			}
		}

		//Case2.1 : all core points are noise 
		if(cluster_core == 0 && noise_core > 0)
		{
			//find all noise points 

			map<int, int> ids;
			vector<double> mean;
			for(point p: epsilon_neighbourhood)
			{
				if(p.clusterId == noiseClusterId)
				{
					ids[p.id] = p.neighCount;
					clusters[p.clusterId].deleteId(p.id);
					//TODO: what if noise is now empty and mean updation
					
				}
			}

			//Crreate new cluster of points
			(Cluster::cnt)++;
			Cluster cluster = Cluster("Cluster_" + to_string(Cluster::cnt), ids, false, Cluster::cnt , mean);

			// add new cluster to subspace
			clusters[Cluster::cnt] = cluster;
		}
		else if(cluster_core > 0) 
		{
			set<int> neighCores;
			for(point p: epsilon_neighbourhood)
			{
				if(p.neighCount >= (int)m_minPts)
				{
					if(p.clusterId != noiseClusterId)
						neighCores.insert(p.clusterId);
				}
			}

			//Case 2.2 add to a pre-existing cluster
			if(neighCores.size()==1)
			{
				//add point to cluster
				int clusterId = *neighCores.begin();
				clusters[clusterId].insertId(m_id, epsilon_neighbourhood.size());


				//TODO: add noise points to cluster if point is a core point

			}
			//case 2.3 Merge Clusters or add to random cluster
			else 
			{
				/*
				if(epsilon_neighbourhood.size()<m_minPts)
				{
					int Cid = *cluster_count.begin();

					//add point to the cluster
					counts.insert(make_pair(m_id,make_pair(epsilon_neighbourhood.size(),Cid)));
					set<pair<int,int>> ids = clusters[Cid].getIds();
					ids.insert(make_pair(m_id,epsilon_neighbourhood.size()));
					clusters[Cid].setIds(ids);

					//add cluster to subspace
					m_subspace.setClusters(clusters);
					m_subspace.setNeighCounts(counts);
				}
				else 
				{
					//create an empty cluster
					set<pair<int,int>> ids;
					vector<double> mean(m_subspace.getDimensionality(),0);
					int Cid = m_subspace.getNext();
					m_subspace.incrNext();
					Cluster cluster = Cluster("Merged Cluster",ids,false,Cid,mean);

					//add old clusters into new cluster and delete old
					for(int i: cluster_count)
					{
						set<pair<int,int>> c_ids = clusters[i].getIds();
						for(pair<int,int> i: c_ids)
						{
							counts[i.first].second = Cid;
						}

						ids.insert(c_ids.begin(),c_ids.end());

						clusters.erase(i);
					}

					// add new point into cluster
					counts.insert(make_pair(m_id,make_pair(epsilon_neighbourhood.size(),Cid)));
					ids.insert(make_pair(m_id,epsilon_neighbourhood.size()));

					// noise points in the neighbourhood to new cluster 
					for(int i:epsilon_neighbourhood)
					{
						if(clusters[counts[i].second].isNoise())
						{
							clusters.erase(counts[i].second);
							counts[i].second = Cid;
							ids.insert(make_pair(i,counts[i].first));
						}
					}

					//add cluster to subspace
					// cluster.setIds(ids);
					m_subspace.setClusters(clusters);
					m_subspace.setNeighCounts(counts);
				}    
				*/
			}
		}
	}

	return m_subspace;
}

Subspace INCRDBSCAN::Delete()
{
	//useful declarations
	int noiseClusterId = m_subspace.getNoiseClusterId();
	map<int,Cluster>& clusters = m_subspace.getClusters();
	bool potnetialSplit = false;
	//cluster of point to delete
	int Cid = noiseClusterId;
	int treshold = 2;

	//find epsilon neighbourhood and delete point
	vector<point> epsilon_neighbourhood = getAndDecrementNeighbourhood(&Cid);

	//Case1: Core Point
	if(epsilon_neighbourhood.size()>=m_minPts)
	{
		for(auto p : epsilon_neighbourhood)
		{
			if(p.neighCount<m_minPts)
			{
				//delete point 
				clusters[p.clusterId].deleteId(p.id);

				int clusterId = getNearestCorePointClusterId(p);
				//make point noise
				if(clusterId==-1)
					clusterId = noiseClusterId;
				//add to the new cluster
				clusters[clusterId].insertId(p.id,p.neighCount);

				//Case of potential split
				if(clusterId != p.clusterId)
					potnetialSplit = true;

			}
		}
	}
	//Case2: Not a core point
	else 
	{
		vector<point> pointsLostCore = getPointsLostCore(epsilon_neighbourhood);

		//If points lost core property
		if(pointsLostCore.size()>0)
		{
			//add points to check
			set<point> pointsToCheck;
			for(point p : pointsLostCore)
			{
				addPointsToCheck(p,pointsToCheck);
			}

			//for each point add to nearest core cluster point or to noise cluster
			for(auto p : pointsToCheck)
			{
				int clusterId = getNearestCorePointClusterId(p);
				if(clusterId==-1)
					clusterId = noiseClusterId;
				clusters[clusterId].insertId(p.id,p.neighCount);

				if(clusterId != p.clusterId)
					potnetialSplit = true;
			}
		}
	}

	//increment if potential split
	if(Cid != noiseClusterId && potnetialSplit)
	{
		clusters[Cid].incrementSplit();
	}

	//check for split condition and run static DBSCAN
	if(Cid != noiseClusterId && clusters[Cid].getSplit() >= treshold)
	{
		Relation<double> dataBase;
		vector<int> idKeys = clusters[Cid].getIdKeys();	
		for(int i:idKeys)
		{
			dataBase.push_back(m_points[i]);
		}
		DBSCAN dbscan = DBSCAN(dataBase,m_subspace,m_eps,m_minPts,m_ids);
		vector<Cluster> splitClusters = dbscan.getClusters();

		//update if the clusters split
		if(splitClusters.size()>1)
		{
			clusters.erase(Cid);
			for(Cluster cluster : splitClusters)
			{
				int clusterId = cluster.getClusterId();
	    		clusters.insert(make_pair(clusterId, cluster));
			}
		}
	}

	return m_subspace;
}

vector<point> INCRDBSCAN::getAndIncrementNeighbourhood()
{
	vector<point> neighbourhood;
	map<int, Cluster>& clusters = m_subspace.getClusters();

	for(auto& cluster : clusters)
	{
		map<int, int>& ids = cluster.second.getIds();
		for(auto& id : ids)
		{
			int i = id.first;
			if(i != m_ids[m_point] && dist(m_point, m_points[i]) <= m_eps)
			{
				id.second++;
				neighbourhood.push_back(point(m_ids[m_points[i]], id.second, cluster.first));
			}
			else if(i == m_ids[m_point])
			{
				clusters[cluster.first].deleteId(i);
			}
		}

	}
	return neighbourhood;
}

vector<point> INCRDBSCAN::getAndDecrementNeighbourhood(int *pCid)
{
	vector<point> neighbourhood;
	map<int, Cluster>& clusters = m_subspace.getClusters();

	for(auto& cluster : clusters)
	{
		map<int, int>& ids = cluster.second.getIds();
		for(auto& id : ids)
		{
			int i = id.first;
			if(dist(m_point, m_points[i]) <= m_eps && i != m_ids[m_point])
			{
				id.second--;
				neighbourhood.push_back(point(m_ids[m_points[i]], id.second, cluster.first));
			}
			else if(m_ids[m_point] == i )
			{
				//delete point and note the cluster the point is part of
				*pCid = cluster.first;
			}
		}

	}
	clusters[*pCid].deleteId(m_ids[m_point]);
	return neighbourhood;
}

vector<point> INCRDBSCAN::getPointsLostCore(vector<point> epsilonNeighbourhood)
{
	vector<point> pointsLostCore;
	for(point p : epsilonNeighbourhood)
	{
		if(p.neighCount == m_minPts-1)
		{
			pointsLostCore.push_back(p);
		}
	}
	return pointsLostCore;
}

int INCRDBSCAN:: getNearestCorePointClusterId(point p)
{
	double min_dis = 10000000000;
	int clusterId = -1;
	
	map<int, Cluster> clusters = m_subspace.getClusters();
	for(auto cluster : clusters)
	{
		map<int, int> ids = cluster.second.getIds();
		double dis;
		for(auto id : ids)
		{
			int i = id.first;
			if(p.id != i && (dis = dist(m_points[p.id], m_points[i])) <= m_eps && id.second >= m_minPts && dis<min_dis)
			{
				dis = min_dis;
				clusterId = cluster.first;
			}
		}
	}
	return clusterId;
}

void INCRDBSCAN::addPointsToCheck(point p,set<point> &pointsToCheck)
{
	map<int, Cluster>& clusters = m_subspace.getClusters();

	map<int, int>& ids = clusters[p.clusterId].getIds();
	for(auto& id : ids)
	{
		int i = id.first;
		if(dist(m_points[p.id], m_points[i]) <= m_eps && id.second< m_minPts)
		{
			clusters[p.clusterId].deleteId(i);
			pointsToCheck.insert(point(id.first,id.second,p.clusterId));
		}
	}
}


double INCRDBSCAN::dist(vector<double>& p1, vector<double>& p2) 
{
	double d = 0;
	for (uint i = 0; i < (uint) m_subspace.getDimensions().size(); ++i) 
	{
		if (m_subspace.hasDimension(i)) 
		{
			d += ((long double)p1[i] - p2[i]) * (p1[i] - p2[i]);
		}
	}
	return sqrt(d);
}
