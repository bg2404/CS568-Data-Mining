#ifndef SUBSPACE_H
#define SUBSPACE_H

#include<iostream>
#include<vector>

using namespace std;

class Subspace
{
	private:
		vector<int> dimensions;
		int dimensionality;

	public:
		// Create a 1-dimensional subspace
		Subspace(int dimension);

		// Default Constructor
		Subspace();

		// Create a subspace with given dimensions
		Subspace(vector<int>& dimensions);

		// Getters
		vector<int>& getDimensions();
		int getDimensionality();

		// Joins subspaces to create new subspace
		Subspace join(Subspace& other);
		vector<int> joinLastDimensions(Subspace& other);

		// Checks if the space is a subspace of the argument or not
		bool isSubspace(Subspace& subspace);

		// Displays dimensions and dimensionality
		void print();
		bool isValid();
};

#endif
