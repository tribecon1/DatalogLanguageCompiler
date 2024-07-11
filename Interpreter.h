#pragma once

#include <iostream>
#include <string>
#include <utility>
using std::string;

#include "ParserObjects.h"
#include "Database.h"
#include "Scheme.h"
#include "Tuple.h"


class Interpreter{

private:
    Datalog givenDatalog;
    Database newDatabase;

public:
    Interpreter(Datalog givenDatalog, Database emptyDatabase) : givenDatalog(std::move(givenDatalog)), newDatabase(std::move(emptyDatabase)) {}


    //modifying methods below a method called 'fillDatabase,' which returns the mutated 'newDatabase'

    [[nodiscard]] Database interpret(){

        schemeEval();
        factEval();
        newDatabase.toString();
        return newDatabase;
    }


    void schemeEval(){
        for (Predicate pre_scheme : givenDatalog.getSchemes()){
            Scheme scheme = Scheme(pre_scheme.convertParametersToStrings());
            newDatabase.addToDatabase(Relation(pre_scheme.getName(), scheme));
        }
    }

    void factEval(){
        for (Predicate fact : givenDatalog.getFacts()){
            Relation& foundRelation = newDatabase.locateRelation(fact.getName());
            foundRelation.addTuple(Tuple(fact.convertParametersToStrings()));
        }
    }

    void queryEval(){
        for (Predicate query : givenDatalog.getQueries()){
            Relation& foundRelation = newDatabase.locateRelation(query.getName());


        }
    }


};