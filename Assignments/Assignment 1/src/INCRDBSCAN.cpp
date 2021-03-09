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
			if(dist(m_point, m_points[i]) <= m_eps)
			{
				id.second++;
				neighbourhood.push_back(point(m_ids[m_points[i]], id.second, cluster.first));
			}
		}

	}
	return neighbourhood;
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
