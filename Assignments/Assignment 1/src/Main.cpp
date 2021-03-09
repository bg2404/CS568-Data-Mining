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
map<int, int> idc;

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
				cout << "Iris-Unlabeled.csv 4 0.4\n";
			}
		}
		cout << "Usage: ./subclu {file} {min points} {epsilon}\n";
		return 0;
	}

	string file = argv[1];
	int mnPnts = stoi(argv[2]);
	double epsilon = stod(argv[3]);

	SUBCLU subclu(file, mnPnts, epsilon);

	
	cout << "Starting SUBCLU run with file = " << file << " ,mnPnts = " << mnPnts << " ,epsilon = " << epsilon << " .....\n";
	map<Subspace, vector<Cluster>> result = subclu.run();

	// Printing Results
	/*
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
	*/

	// Testing and Visualisation
	ReadInput reader(file);
	Relation<double> dataBase = reader.read();

	if (!dataBase.size())
		return 0;

	int total_d = dataBase[0].size();

	for (int i = 0; i < (int)(dataBase.size()); i++)
	{
		dbids[dataBase[i]] = i;
	}

	Subspace ss;

	for (auto x : result)
	{
		// bool flag = false;
		string file = "Subspace";
		Subspace subspace = x.first;
		for (auto d : subspace.getDimensions())
		{
			file.push_back((char)(d + '0'));
		}
		if (file == "Subsapce1111.csv")
		{
			ss = subspace;
			// flag = true;
		}
		file = file + ".csv";

		fstream p_file;
		p_file.open(file, ios::out);

		int label = 0;

		for (auto y : x.second)
		{
			map<int, int> idPairs = y.getIds();
			set<int> ids; 
			for(auto x : idPairs)
				ids.insert(x.first);
			for (int id : ids)
			{
				idc[id] = label;
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

	//for confusion_matrix for Subspace1111.csv database
	string filename = "confusion_matrix.csv";
	fstream p_file;
	p_file.open(filename, ios::out);
	for (int i = 0; i < (int)(dataBase.size()); i++)
	{
		for (int j = 0; j < total_d; j++)
		{
			p_file << dataBase[i][j] << " ";
		}
		if (idc.find(i) != idc.end())
		{
			p_file << idc[i] << "\n";
		}
		else
		{
			p_file << "-1\n";
		}
	}
	return 0;
}
