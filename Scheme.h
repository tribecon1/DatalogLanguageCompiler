#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "ParserObjects.h"

class Scheme : public vector<string> {

public:

    Scheme(vector<string> names) : vector<string>(names) { }

};