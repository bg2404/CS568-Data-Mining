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

INCRDBSCAN::INCRDBSCAN(vector<double> point, double eps,uint minPts,Relation<double> points,Subspace &subspace, int id)
{
    m_point = point;
    m_eps = eps;
    m_minPts = minPts;
    m_points = points;
    m_subspace = subspace;
    m_id = id;
}

Subspace INCRDBSCAN::Insert()
{
    bool core_p = false;   
    // find epsilon neighbourhood 
    vector<int> epsilon_neighbourhood = get_neighbourhood(m_point);

    //increment count of each by 1
    map<int,pair<int,int>> counts = m_subspace.get_counts();
    map<int,Cluster> clusters = m_subspace.get_Clusters();

    //increment count in epsilon neighbourhood 
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
        //Create noise new clulster
        set<int> ids;
        ids.insert(m_id);
        string name = "New Cluster";
        Cluster cluster = Cluster(name,ids,true,m_subspace,m_point);

        //add noise to subspace
        clusters.insert(make_pair(cluster.size(),cluster));
        counts.insert(make_pair(m_id,make_pair(epsilon_neighbourhood.size(),clusters.size()-1)));
        m_subspace.set_Clusters_Counts(clusters,counts);
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

            if(clusters[counts[i].second].isNoise())
            {
                noise_core++;
            }
            else 
            cluster_core++;
        }
        //Case2.1 : all core points are noise 
        if(cluster_core==0 && noise_core>0)
        {
            //find all noise points and calculate mean
            set<int> noise;
            int dim = m_subspace.getDimensionality();
            vector<double> mean(dim,0);
            for(int i: epsilon_neighbourhood)
            {
                if(clusters[counts[i].second].isNoise())
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

            //Crreate new cluster of points
            Cluster cluster = Cluster("New Cluster",noise,false,m_subspace,mean);
            int Cid = clusters.size();

            // add new cluster to subspace
            clusters.insert(make_pair(Cid,cluster));
            for(int i:noise)
            {
                clusters.erase(counts[i].second);
                counts[i].second = Cid;
            }
            counts.insert(make_pair(m_id,make_pair(epsilon_neighbourhood.size(),Cid)));
            m_subspace.set_Clusters_Counts(clusters,counts);
        }
        else if(cluster_core>0) 
        {
            set<int> cluster_count;
            for(int i:epsilon_neighbourhood)
            {
                if(counts[i].first>=m_minPts && !clusters[counts[i].second].isNoise())
                {
                    cluster_count.insert(counts[i].second);
                }
            }

            //Case 2.2 add to a pre-existing cluster
            if(cluster_count.size()==1)
            {
                //obtain the cluster
                int Cid = *cluster_count.begin();
                Cluster cluster = clusters[Cid];
                set<int> ids = cluster.getIds();

                //add point to cluster
                ids.insert(m_id);
                cluster.setIds(ids);
                counts.insert(make_pair(m_id,make_pair(epsilon_neighbourhood.size(),Cid)));

                //add noise points to cluster if point is a core point
                if(epsilon_neighbourhood.size()>0)
                {
                    for(int i: epsilon_neighbourhood)
                    {
                        if(clusters[counts[i].second].isNoise())
                        {
                            ids.insert(i);
                            clusters.erase(counts[i].second);
                            counts[i].second = Cid;
                        }
                    }

                    cluster.setIds(ids);
                }

                //add cluster to subspace
                clusters[Cid] = cluster;
                m_subspace.set_Clusters_Counts(clusters,counts);

            }
            //case 2.3 Merge Clusters or add to random cluster
            else 
            {
                if(epsilon_neighbourhood.size()<m_minPts)
                {
                    int Cid = *cluster_count.begin();
                    
                    //add point to the cluster
                    counts.insert(make_pair(m_id,make_pair(epsilon_neighbourhood.size(),Cid)));
                    set<int> ids = clusters[Cid].getIds();
                    ids.insert(m_id);
                    clusters[Cid].setIds(ids);

                    //add cluster to subspace
                    m_subspace.set_Clusters_Counts(clusters,counts);
                }
                else 
                {
                    //create an empty cluster
                    set<int> ids;
                    vector<double> mean(m_subspace.getDimensionality(),0);
                    Cluster cluster = Cluster("Merged Cluster",ids,false,m_subspace,mean);
                    int Cid = clusters.size();

                    //add old clusters into new cluster and delete old
                    for(int i: cluster_count)
                    {
                        set<int> c_ids = clusters[i].getIds();
                        for(int i: c_ids)
                        {
                            counts[i].second = Cid;
                        }

                        ids.insert(c_ids.begin(),c_ids.end());

                        clusters.erase(i);
                    }

                    // add new point into cluster
                    counts.insert(make_pair(m_id,make_pair(epsilon_neighbourhood.size(),Cid)));
                    ids.insert(m_id);

                    // noise points in the neighbourhood to new cluster 
                    for(int i:epsilon_neighbourhood)
                    {
                        if(clusters[counts[i].second].isNoise())
                        {
                            clusters.erase(counts[i].second);
                            counts[i].second = Cid;
                            ids.insert(i);
                        }
                    }

                    //add cluster to subspace
                    cluster.setIds(ids);
                    clusters.insert(make_pair(Cid,cluster));
                    m_subspace.set_Clusters_Counts(clusters,counts);
                }    
            }
        }
    }

    return m_subspace;
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
