#pragma once

#include <iostream>
#include <string>
#include <utility>
using std::string;
#include <unordered_map>
using std::unordered_map;

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



    [[nodiscard]] Database interpret(){

        schemeEval();
        factEval();
        //newDatabase.toString();
        //std::cout << "------------" << std::endl;
        queryEval();

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
            Relation foundRelation = newDatabase.locateRelation(fact.getName());
            foundRelation.addTuple(Tuple(fact.convertParametersToStrings()));
            newDatabase.addToDatabase(foundRelation);
        }
    }

    static vector<int> repeatVariableIndexes(int start, const vector<Parameter>& listOfParams){
        vector<int> repeatVarIndexes;
        repeatVarIndexes.push_back(start);
        for (int index = start+1; index < listOfParams.size(); index++){
            if(listOfParams.at(index).toString() == listOfParams.at(start).toString() ){
                repeatVarIndexes.push_back(index);
            }
        }
        return repeatVarIndexes;
    }


    void queryEval(){
        for (Predicate query : givenDatalog.getQueries()){
            unordered_map<string, vector<int>> variableAndIndexes;
            vector<string> renamed_columns;

            Relation modifiedRelation = newDatabase.locateRelation(query.getName());

            for (int paramIndex = 0; paramIndex < query.getParameters().size(); paramIndex++){
                if(query.getParameters().at(paramIndex).isTypeID()){
                    if (variableAndIndexes.find(query.getParameters().at(paramIndex).toString()) == variableAndIndexes.end()){
                        string variable = query.getParameters().at(paramIndex).toString();
                        renamed_columns.push_back(variable);

                        vector<int> varIndexes = repeatVariableIndexes(paramIndex, query.getParameters());
                        variableAndIndexes[variable] = varIndexes;
                    }

                }
                else{
                    string constant = query.getParameters().at(paramIndex).toString();
                    modifiedRelation = modifiedRelation.select(paramIndex, constant);
                }
            }

            for (const auto& pair : variableAndIndexes) {
                modifiedRelation = modifiedRelation.select(pair.second);
            }



            std::set<int> relevantColumns; //use set to keep proper order of variables as they appear
            if (variableAndIndexes.size() > 1){
                for (const auto& pair : variableAndIndexes){
                    for (int index : pair.second){
                        relevantColumns.insert(index);
                    }
                }
                modifiedRelation = modifiedRelation.project(relevantColumns);
            }
            else if (variableAndIndexes.size() == 1){
                for (const auto& pair : variableAndIndexes){
                    relevantColumns.insert(pair.second.front());
                }
                modifiedRelation = modifiedRelation.project(relevantColumns);
            }
            else{
                //nothing to remove for columns, all constants
            }

            modifiedRelation = modifiedRelation.rename(Scheme(renamed_columns));


            //printing
            string header;
            header = query.toString() + "? ";
            if (modifiedRelation.getTupleCount() == 0){
                header += "No";
            }
            else{
                header += "Yes(" + std::to_string(modifiedRelation.getTupleCount()) + ")";
            }
            std::cout << header << std::endl;
            if (!variableAndIndexes.empty()){
                std::cout << modifiedRelation.toString();
            }


        }
    }


};