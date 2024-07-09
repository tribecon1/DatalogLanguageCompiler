#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;

class Scheme : public vector<string> {

public:

    Scheme(vector<string> names) : vector<string>(names) { }

};