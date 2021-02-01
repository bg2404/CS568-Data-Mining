#include<iostream>
#include<vector>
#include "Subspace.h"

using namespace std;

int main()
{
	vector<Subspace> s;
	vector<Subspace> result;
	int n = 10;
	for(int i = 0; i < n; i++)
		s.push_back(Subspace(i + 1));

	for(int i = 0; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			Subspace temp = s[i].join(s[j]);
			if(temp.isValid())
			{
				result.push_back(temp);
			}
		}
	}


	for(auto x : result)
	{
		x.print();
		cout << "_______________________________\n";
	}
	return 0;
}
