#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
#include "ReadInput.h"
#include "Relation.h"

using namespace std;

std::string trim(string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

ReadInput::ReadInput(string file)
{
    input_file.open(file, ios::in);
}

Relation ReadInput::read()
{
    Relation data;
    vector<float> row;
    string line;

    while (input_file)
    {
        //read a line
        getline(input_file, line);

        //trim left and right
        line = trim(line);

        if (line == "" || line == "\n")
            continue;

        //devide into different words
        istringstream ss(line);

        //convert each word to float and add to row
        string word;
        while (ss >> word)
        {
            row.push_back(stof(word));
        }
        //add to data
        data.push_back(row);
        row.clear();
    }
    return data;
}