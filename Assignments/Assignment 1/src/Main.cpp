#include <iostream>
#include <set>
#include <vector>

#include "DBSCAN.h"
#include "Cluster.h"
#include "ReadInput.h"
#include "Subspace.h"
#include "SUBCLU.h"

using namespace std;

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		if(argc == 2)
		{
			string help = argv[1];
			if(help == "help")
			{
				cout << "min points and epsilon values that work....\n";
				cout << "mouse.csv 12 0.05\n";
				cout << "cube.csv  1   70\n";
			}
		}
		cout << "Usage: ./subclu {file} {min points} {epsilon}\n";
		return 0;
	}

	string file = argv[1];
	int mnPnts = stoi(argv[2]);
	double epsilon = stod(argv[3]);

	SUBCLU subclu(file, mnPnts, epsilon);

	map<Subspace, vector<Cluster>> result = subclu.run();

	for (auto x : result)
	{
		cout << "############################################\n";
		Subspace subspace = x.first;
		subspace.print();
		cout << "############################################\n";

		for (auto y : x.second)
		{
			y.print();
			cout << "-----------------------------------------------\n";
		}
		cout << "////////////////////////////////////////////\n";
	}

	//TODO: Plotting results and testing...

	return 0;
}
