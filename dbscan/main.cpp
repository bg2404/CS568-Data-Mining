#include <stdio.h>

#include <boost/algorithm/string.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <fstream>
#include <iostream>
#include <vector>

#include "dbscan.h"

namespace bg = boost::geometry;

using namespace std;

void readBenchmarkData(string dataFilename, vector<Point> &points, vector<int> &clusterID) {
    // load point cloud
    ifstream fin(dataFilename.c_str(), ios::in);

    string line;

    while (std::getline(fin, line)) {
        vector<string> strings;
        boost::split(strings, line, boost::is_any_of(","));
        Point p;
        bg::set<0>(p, stod(strings[0]));
        bg::set<1>(p, stod(strings[1]));
        bg::set<2>(p, stod(strings[2]));
        points.push_back(p);
        clusterID.push_back(stod(strings[3]));
    }
    fin.close();
}

void publishResults(vector<Point> points, vector<int> clusterID, vector<int> pred_clusterID, int num_points) {
    int i = -1;
    ofstream fout("a.csv", ios::out);
    int c = 23;
    for (size_t i = 0; i < 3; ++i) {
        fout << char('a' + c) << ',';
        c = (c + 1) % 26;
    }
    fout << "pred,label\n";
    while (++i < num_points) {
        fout << bg::get<0>(points[i]) << ',';
        fout << bg::get<1>(points[i]) << ',';
        fout << bg::get<2>(points[i]) << ',';
        fout << pred_clusterID[i] << ","
             << clusterID[i] << '\n';
    }
    fout.close();
}

int main(int argc, char *argv[]) {
    double eps;
    int minPts;
    string dataFilename;
    if (argc < 4) {
        eps = 1.0;
        minPts = 4;
        dataFilename = "hepta.dat";
    } else {
        eps = stof(argv[1]);
        minPts = stoi(argv[2]);
        dataFilename = argv[3];
    }

    vector<Point> points;
    vector<int> clusterID;

    readBenchmarkData(dataFilename, points, clusterID);
    DBSCAN ds(points, eps, minPts);
    vector<int> pred_clusterID = ds.getClusters();
    publishResults(points, clusterID, pred_clusterID, ds.getNumPoints());
    return 0;
}