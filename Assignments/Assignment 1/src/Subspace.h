#ifndef SUBSPACE_H
#define SUBSPACE_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Subspace
{
private:
    vector<int> dimensions;
    int dimensionality;
    map<int,pair<int,int>> counts;

public:
    // Create a 1-dimensional subspace
    Subspace(int dimension);

    // Default Constructor
    Subspace();

    // Create a subspace with given dimensions
    Subspace(vector<int> &dimensions);

    // Getters
    vector<int> &getDimensions();
    int& getDimensionality();

    // Add or Remove dimensions
    void addDimension(int dimension);
    void removeDimension(int dimension);

    // Joins subspaces to create new subspace
    Subspace join(Subspace &other);
    vector<int> joinLastDimensions(Subspace &other);

    // Checks if the space is a subspace of the argument or not
    bool isSubspace(Subspace &subspace);

    // Checks if ith dimension is present in the subspace
    bool hasDimension(int i);

    // Displays dimensions and dimensionality
    void print();
    bool isValid();

    //defining < operator
    bool operator<(const Subspace &s2) const;

    //set map counts
    void set_counts(map<int,pair<int,int>> counts);

    //return counts
    map<int,pair<int,int>> get_counts();
};

#endif
