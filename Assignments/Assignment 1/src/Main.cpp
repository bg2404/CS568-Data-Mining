#include <iostream>
#include <set>
#include <vector>

#include "Cluster.h"
#include "ReadInput.h"
#include "Subspace.h"

using namespace std;

int main() {
    ReadInput r("input.txt");
    Relation<double> relation = r.read();
    for (auto x : relation) {
        for (auto y : x)
            cout << y << ' ';
        cout << '\n';
    }
    set<int> ids = {1, 3, 4, 9, 10};
    vector<double> mean;
    string name = "MyCluster";
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
