#include<iostream>
#include<vector>
#include<algorithm>
#include "Subspace.h"
#include "BitsUtil.h"

using namespace std;

Subspace::Subspace(int dimension)
{
	(this -> dimensions).assign(dimension + 1, 0);
	(this -> dimensions)[dimension] = 1;
	this -> dimensionality = 1;
}

Subspace::Subspace()
{
	this -> dimensions = vector<int>();
	this -> dimensionality = 0;
}

Subspace::Subspace(vector<int>& dimensions)
{
	this -> dimensions = dimensions;
	this -> dimensionality = count(dimensions.begin(), dimensions.end(), 1);
}

vector<int>& Subspace::getDimensions()
{
	return this -> dimensions;
}

int Subspace::getDimensionality()
{
	return this -> dimensionality;
}

Subspace Subspace::join(Subspace& other)
{
	vector<int> newDimensions = joinLastDimensions(other);
	return Subspace(newDimensions);
}

vector<int> Subspace::joinLastDimensions(Subspace& other)
{
	vector<int> result;
	if((this -> dimensionality) != other.dimensionality)
		return vector<int>();

	if((this -> dimensionality) == 1)
		return BitsUtil::orVectors(this -> dimensions, other.dimensions);	

	if((this -> dimensions).size() >= (other.dimensions).size())
		return vector<int>();

	result = BitsUtil::orVectors(this -> dimensions, other.dimensions);	

	if(count(result.begin(), result.end(), 1) != this -> dimensionality + 1)
		return vector<int>();

	return result;

}

bool Subspace::isSubspace(Subspace& subspace)
{
	if((this -> dimensionality) <= subspace.dimensionality)
	{
		if((this -> dimensionality) == BitsUtil::intersection(this -> dimensions, subspace.dimensions))
			return true;
		return false;

	}
	return false;
}

void Subspace::print()
{
	cout << "dimensionality = " << this -> dimensionality << '\n';
	cout << "dimensions = ";
	for(auto x : this -> dimensions)
		cout << x;
	cout << '\n';
}

bool Subspace::isValid()
{
	return ((this -> dimensionality) != 0 && !(this -> dimensions).empty());
}

