#ifndef ReadInput_H
#define ReadInput_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Relation.h"

using namespace std;

class ReadInput
{
private:
    ifstream input_file;

public:
    //open the file
    ReadInput(string file);

    //read input
    Relation read();
};

#endif