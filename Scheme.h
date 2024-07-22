#pragma once

#include <string>
using std::string;
#include <utility>
#include <vector>
using std::vector;

#include "ParserObjects.h"

class Scheme : public vector<string> {

public:

    explicit Scheme(vector<string> names) : vector<string>(std::move(names)) { }

};