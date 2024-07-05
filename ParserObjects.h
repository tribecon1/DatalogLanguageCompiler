#ifndef PARSEROBJECTS_H
#define PARSEROBJECTS_H

#include "Token.h"
#include <vector>
using std::vector;
#include <set>
using std::set;

class Parameter {
private:
    Token paramToken;

public:
    explicit Parameter(Token IDorString) : paramToken(IDorString) {}

    bool isTypeID(){
        if (paramToken.getType() == Token::ID){
            return true;
        }
        else{
            return false;
        }
    }
};



class Predicate {

private:
    std::string name;
    std::vector<Parameter> parameters;

public:
    explicit Predicate(std::string givenName) : name(givenName) {}

    void addParameter(const Parameter& givenParameter){
        parameters.push_back(givenParameter);
    }

    void addParameter(const std::vector<Parameter>& givenParameters){
        for (Parameter currParameter : givenParameters){
            parameters.push_back(currParameter);
        }
    }
};



class Rule {

private:
    Predicate headPredicate;
    std::vector<Predicate> bodyPredicates;

public:
    explicit Rule(Predicate givenHeadPredicate) : headPredicate(givenHeadPredicate) {}

    void addPredicates(const Predicate& givenFirstPredicate){
        bodyPredicates.push_back(givenFirstPredicate);
    }

    void addPredicates(const std::vector<Predicate>& givenPredicates){
        for (Predicate currPredicate : givenPredicates){
            bodyPredicates.push_back(currPredicate);
        }
    }

};



class Datalog {

private:
    std::vector<Predicate> facts;
    std::vector<Rule> rules;
    std::vector<Predicate> schemes;
    std::vector<Predicate> queries;

    std::set<std::string> domain;

public:
    void addToFacts(const Predicate& new_fact){
        facts.push_back(new_fact);
    }
    void addToRules(const Rule& new_rule){
        rules.push_back(new_rule);
    }
    void addToSchemes(const Predicate& new_scheme){
        schemes.push_back(new_scheme);
    }
    void addToQueries(const Predicate& new_query){
        queries.push_back(new_query);
    }
    void addToDomain(const std::string& attempted_newstring){
        domain.insert(attempted_newstring);
    }

};



#endif