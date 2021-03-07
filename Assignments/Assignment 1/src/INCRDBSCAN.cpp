#include "INCRDBSCAN.h"

#include <assert.h>

#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <stdlib.h>

#include "Cluster.h"
#include "Relation.h"
#include "Subspace.h"

using namespace std;

INCRDBSCAN::INCRDBSCAN(vector<double> point, double eps,uint minPts,Relation<double> points, vector<Cluster> clusters,Subspace subspace, int id)
{
    m_point = point;
    m_eps = eps;
    m_minPts = minPts;
    m_clusters = clusters;
    m_points = points;
    m_subspace = subspace;
    m_id = id;
}

vector<Cluster> INCRDBSCAN::Insert()
{
    bool core_p = false;   
    // find epsilon neighbourhood 
    vector<int> epsilon_neighbourhood = get_neighbourhood(m_point);

    //increment count of each by 1
    map<int,pair<int,int>> counts = m_subspace.get_counts();
    for(int i:epsilon_neighbourhood)
    {
        pair<int,int> c = counts[i];
        c.first = c.first+1;
        counts[i] = c;
        if(c.first>=m_minPts)
        {
            core_p = true;
        }
    }

    //case1 
    if(!core_p)
    {
        set<int> ids;
        ids.insert(m_id);
        string name = "New Cluster";
        Cluster cluster = Cluster(name,ids,true,m_subspace,m_point);
        counts.insert(make_pair(m_id,make_pair(epsilon_neighbourhood.size(),m_clusters.size())));
        m_clusters.push_back(cluster);

    }
    //case2
    else 
    {
        int noise_core = 0;
        int cluster_core = 0;
        for(int i: epsilon_neighbourhood)
        {
            if(counts[i].first<m_minPts)
            continue;

            if(m_clusters[counts[i].second].isNoise())
            {
                noise_core++;
            }
            else 
            cluster_core++;
        }
        //all core points are noise 
        if(cluster_core==0 && noise_core>0)
        {
            //find all noise points
            set<int> noise;
            int dim = m_subspace.getDimensionality();
            vector<double> mean(dim,0);
            for(int i: epsilon_neighbourhood)
            {
                if(m_clusters[counts[i].second].isNoise())
                {
                    noise.insert(i);
                    for(int j=0;j<dim;j++)
                    {
                        mean[j]+=m_points[i][j];
                    }
                }
            }
            noise.insert(m_id);
            for(int j=0;j<dim;j++)
            {
                mean[j]+=m_point[j];
                mean[j]/=noise.size();
            }
            //make new cluster with them
            Cluster cluster = Cluster("New Cluster",noise,false,m_subspace,mean);
            m_clusters.push_back(cluster);
        }
        else if(cluster_core>0) 
        {
            for(int i:epsilon_neighbourhood)
            {
                
            }
        }
        //core points belong to single cluster
        //points belong to different clusters
    }
}

vector<int> INCRDBSCAN::get_neighbourhood(vector<double> p)
{
    vector<int> neighbourhood;
    for(int i=0;i<m_points.size();i++)
    {
        if(dist(p,m_points[i]) <= m_eps)
        {
            neighbourhood.push_back(m_ids[m_points[i]]);
        }
    }
    return neighbourhood;
}

double INCRDBSCAN::dist(vector<double> p1, vector<double> p2) 
{
	double d = 0;
	for (uint i = 0; i < m_subspace.getDimensions().size(); ++i) 
	{
		if (m_subspace.hasDimension(i)) 
		{
			d += ((long double)p1[i] - p2[i]) * (p1[i] - p2[i]);
		}
	}
	return sqrt(d);
}
