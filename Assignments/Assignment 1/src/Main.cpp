#include <iostream>
#include <set>
#include <vector>

#include "DBSCAN.h"
#include "Cluster.h"
#include "ReadInput.h"
#include "Subspace.h"

using namespace std;

int main() {
    ReadInput r("data.csv");
    Relation<double> relation = r.read();
    for (auto x : relation) {
        for (auto y : x)
            cout << y << ' ';
        cout << '\n';
    }
    

    vector<int> space = {1, 0, 1};
    Subspace subs(space);
    vector<double> mean;

    map<vector<double>, int> id;
    for(int i = 0; i < relation.size(); i++)
	    id[relation[i]] = i;

    subs.print();
    DBSCAN dbscan(relation, subs, 10.0, 1, id);
    vector<Cluster> cl = dbscan.getClusters();

    for(auto x : cl)
    {
	    x.print();
	    cout << "----------------\n";
    }
    string name = "MyCluster";
    
    set<int> ids = {1, 3, 4, 9, 10};
    Subspace s3(3);
    Cluster c(name, ids, false, s3, mean);
    c.print();
    cout << "-----------------------\n";

    vector<Subspace> s;
    vector<Subspace> result;
    int n = 10;
    for (int i = 0; i < n; i++)
        s.push_back(Subspace(i + 1));

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Subspace temp = s[i].join(s[j]);
            if (temp.isValid()) {
                result.push_back(temp);
            }
        }
    }

    for (auto x : result) {
        x.print();
        cout << "_______________________________\n";
    }
    return 0;
}
