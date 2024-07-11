#pragma once

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

    Relation& locateRelation(const string& relation_name){
        return relations_database.at(relation_name);
    }

    unsigned getRelationCount(){
        return relations_database.size();
    }

    void toString(){
        for (const auto& pair : relations_database){
            std::cout << "SCHEME \'" << pair.first << "\': " << std::endl;
            std::cout << pair.second.toString() << std::endl;
        }
    }

};