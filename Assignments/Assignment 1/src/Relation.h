#ifndef Relation_H
#define Relation_H

#include <vector>

using namespace std;

class Relation : public vector<vector<float>>
{
    int get_dimensionality();
};

#endif