#ifndef PARSEROBJECTS_H
#define PARSEROBJECTS_H

#include "Token.h"
#include <vector>
using std::vector;
#include <set>
using std::set;
#include <string>
#include <sstream>
using std::stringstream;

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

    [[nodiscard]] std::string toString() const{
        return paramToken.getValue();

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

    [[nodiscard]] std::string toString() const{
        stringstream ss;
        ss << name << "(";
        for (int index = 0; index < parameters.size()-1; index++){
            ss << parameters.at(index).toString() << ",";
        }
        ss << parameters.at(parameters.size()-1).toString() << ")";
        return ss.str();
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

    [[nodiscard]] std::string toString(){
        stringstream ss;
        ss << headPredicate.toString() << " :- ";
        for (int index = 0; index < bodyPredicates.size()-1; index++){
            ss << bodyPredicates.at(index).toString() << ",";
        }
        ss << bodyPredicates.at(bodyPredicates.size()-1).toString() << ".";
        return ss.str();
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

    [[nodiscard]] const vector<Predicate> &getFacts() const {
        return facts;
    }
    [[nodiscard]] const vector<Rule> &getRules() const {
        return rules;
    }
    [[nodiscard]] const vector<Predicate> &getSchemes() const {
        return schemes;
    }
    [[nodiscard]] const vector<Predicate> &getQueries() const {
        return queries;
    }
    [[nodiscard]] const set<std::string> &getDomain() const {
        return domain;
    }

    [[nodiscard]] std::string toString(){
        stringstream ss;
        ss << "Success!\n";
        ss << "Schemes(" << schemes.size() << "):\n";
        for (Predicate currPredicate : schemes){
            ss << "  " << currPredicate.toString() << "\n";
        }
        ss << "Facts(" << facts.size() << "):\n";
        for (Predicate currPredicate : facts){
            ss << "  " << currPredicate.toString() << ".\n";
        }
        ss << "Rules(" << rules.size() << "):\n";
        for (Rule currRule : rules){
            ss << "  " << currRule.toString() << "\n";
        }
        ss << "Queries(" << queries.size() << "):\n";
        for (Predicate currPredicate : queries){
            ss << "  " << currPredicate.toString() << "?\n";
        }
        ss << "Domain(" << domain.size() << "):\n";
        for (std::string currString : domain){
            ss << "  " << currString << "\n";
        }
        std::string datalogStringForm = ss.str();
        datalogStringForm.pop_back();
        return datalogStringForm;
    }

};



#endif