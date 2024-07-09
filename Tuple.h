#pragma once

#include <iostream>
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <vector>
using std::vector;

#include "Scheme.h"

class Tuple {

private:

    vector<string> values;

public:

    Tuple(vector<string> values) : values(values) { }

    unsigned size() const {
        return values.size();
    }

    const string& at(int index) const {
        return values.at(index);
    }

    bool operator<(const Tuple& t) const {
        return values < t.values;
    }

    [[nodiscard]] string toString(const Scheme& scheme) const {
        const Tuple& tuple = *this;
        stringstream out;
        for (int index = 0; index < scheme.size(); index++){
            out << scheme.at(index) << "="  << tuple.at(index);
            if (index < scheme.size()-1){
                out << ", ";
            }
        }
        return out.str();
    }

};