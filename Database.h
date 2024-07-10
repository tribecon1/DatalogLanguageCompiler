#include <iostream>
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <unordered_map>
using std::unordered_map;

#include "ParserObjects.h"
#include "Relation.h"

class Database{

private:
    unordered_map<string, Relation> relations_database;

public:
    Database() {}

    void addToDatabase(Relation givenRelation){
        relations_database.insert({givenRelation.getName() ,givenRelation});
    }


};