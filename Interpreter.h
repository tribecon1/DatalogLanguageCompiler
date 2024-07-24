#pragma once

#include <iostream>
#include <string>
#include <utility>
using std::string;
#include <unordered_map>
using std::unordered_map;
#include <queue>
using std::queue;

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
        ruleEval();
        for (const Predicate& query : givenDatalog.getQueries()){
            queryEval(query);
        }

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


    void ruleEval(){
        queue<Relation> relationsToJoin;

        stringstream ruleEvalOutput;

        bool tupleFound = true;
        unsigned iter_count = 1;
        while (tupleFound){
            tupleFound = false;
            for (Rule rule : givenDatalog.getRules()){

                ruleEvalOutput << rule.toString() << "\n";

                for (const Predicate& bodyPred : rule.getBodyPredicates()){
                    relationsToJoin.push(queryEval(bodyPred));
                }
                while (relationsToJoin.size() > 1){
                    Relation leftRelation = relationsToJoin.front();
                    relationsToJoin.pop();
                    Relation rightRelation = relationsToJoin.front();
                    relationsToJoin.pop();
                    Relation joinedRelation = leftRelation.join(rightRelation);
                    relationsToJoin.push(joinedRelation);
                }
                Relation finalJoinedRelation = relationsToJoin.front();
                relationsToJoin.pop();

                vector<int> columnsToProject = columnIndexConverter(rule.getHeadPredicate().getParameters(), finalJoinedRelation.getScheme());
                finalJoinedRelation = finalJoinedRelation.project(columnsToProject);

                Relation finalRelation = newDatabase.locateRelation(rule.getHeadPredicate().getName());
                unsigned prevTupleSize = newDatabase.getTuplesCount();
                finalJoinedRelation = finalJoinedRelation.rename(finalRelation.getScheme());

                finalRelation = finalRelation.relationUnion(finalJoinedRelation, ruleEvalOutput);
                newDatabase.addToDatabase(finalRelation);

                if (newDatabase.getTuplesCount() > prevTupleSize){
                    iter_count++;
                    tupleFound = true;
                }

            }
        }

        std::cout << "Rule Evaluation" << std::endl;
        std::cout << ruleEvalOutput.str() << std::endl;
        std::cout << "Schemes populated after " << iter_count << " passes through the Rules." << std::endl;

    }


    static vector<int> columnIndexConverter(const vector<Parameter>& headPredicateVars, const Scheme& joinedRelationScheme){
        vector<int> columnsToProject;

        for (const Parameter& var : headPredicateVars){
            for (int index = 0; index < joinedRelationScheme.size(); index++){
                if (joinedRelationScheme.at(index) == var.toString()){
                    columnsToProject.push_back(index);
                }
            }
        }

        return columnsToProject;
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


    Relation queryEval(Predicate query){
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



        std::set<int> relevantColumnsOrdered; //use set to keep proper order of variables as they appear
        if (!variableAndIndexes.empty()){
            for (const auto& pair : variableAndIndexes){
                relevantColumnsOrdered.insert(pair.second.front());
            }
            vector<int> relevantColumnsFinal(relevantColumnsOrdered.begin(), relevantColumnsOrdered.end());
            modifiedRelation = modifiedRelation.project(relevantColumnsFinal);
        }
        else{
            //nothing to remove for columns, all constants
        }

        modifiedRelation = modifiedRelation.rename(Scheme(renamed_columns));

        //printing
        /*string header;
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
        }*/

        return modifiedRelation;
    }

};