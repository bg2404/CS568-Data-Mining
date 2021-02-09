#include <iostream>
#include <set>
#include <vector>
#include <fstream>

#include "DBSCAN.h"
#include "Cluster.h"
#include "ReadInput.h"
#include "Subspace.h"
#include "Relation.h"
#include "SUBCLU.h"

using namespace std;

map<vector<double>, int> dbids;

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		if (argc == 2)
		{
			string help = argv[1];
			if (help == "help")
			{
				cout << "min points and epsilon values that work....\n";
				cout << "mouse.csv 12 0.05\n";
				cout << "cube.csv  1   70\n";
				cout << "Iris-Unlabeled.csv 2 0.5\n";
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

	// Printing Results
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

	// Testing and Visualisation
	ReadInput reader(file);
	Relation<double> dataBase = reader.read();

	if(!dataBase.size())
		return 0;

	int total_d = dataBase[0].size();

	for (int i = 0; i < (int)(dataBase.size()); i++)
	{
		dbids[dataBase[i]] = i;
	}

	for (auto x : result)
	{
		string file = "Subspace";
		Subspace subspace = x.first;
		for (auto d : subspace.getDimensions())
		{
			file.push_back((char)(d + '0'));
		}
		file = file + ".csv";

		fstream p_file;
		p_file.open(file, ios::out);

		int label = 0;

		for (auto y : x.second)
		{
			set<int> ids = y.getIds();
			for (int id : ids)
			{
				for (int i = 0; i < total_d; i++)
				{
					if (subspace.hasDimension(i))
					{
						p_file << dataBase[id][i] << " ";
					}
				}
				p_file << label << "\n";
			}
			label++;
		}
		p_file.close();
	}

	return 0;
}
