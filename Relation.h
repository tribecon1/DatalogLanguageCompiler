#include <iostream>
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <vector>
using std::vector;
#include <set>
using std::set;

#include "Scheme.h"
#include "Tuple.h"

class Relation{

private:
    string name;
    set<Tuple> tuples;
    Scheme column_headers;

public:

    Relation(const string& name, const Scheme& scheme) : name(name), column_headers(scheme) { }

    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }

    Relation select(int index, const string& value) const{
        Relation result(name, column_headers);
        // continue!


        return result;
    }


    [[nodiscard]] string toString() const {
        stringstream out;
        for (const Tuple& tuple : tuples) {
            std::cout << tuple.toString(column_headers) << std::endl;
        }
        return out.str();
    }


};