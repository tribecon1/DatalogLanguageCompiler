#pragma once

#include <iostream>
#include <string>
#include <utility>
using std::string;
#include <unordered_map>
using std::unordered_map;
#include <queue>
using std::queue;
#include <stack>
using std::stack;

#include "ParserObjects.h"
#include "Database.h"
#include "Scheme.h"
#include "Tuple.h"
#include "Graph.h"


class Interpreter{

private:
    Datalog givenDatalog;
    Database newDatabase;
    stack<int> postOrder;
    set<int> nodesVisitedForSCC;

public:
    Interpreter(Datalog givenDatalog, Database emptyDatabase) : givenDatalog(std::move(givenDatalog)), newDatabase(std::move(emptyDatabase)) {}


    [[nodiscard]] Database interpret(){

        schemeEval();
        factEval();
        efficientRuleEval();
        for (const Predicate& query : givenDatalog.getQueries()){
            queryEval(query, true);
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


    Graph makeGraph(const vector<Rule>& rules) {
        stringstream out;

        Graph graph(rules.size());

        for (int ruleIndex = 0; ruleIndex < rules.size(); ruleIndex++){
            Rule currRule = rules.at(ruleIndex);
            for (Predicate bodyPred : currRule.getBodyPredicates()){
            }
            for (Predicate bodyPred : currRule.getBodyPredicates()){
                for (int repeatIndex = 0; repeatIndex < rules.size(); repeatIndex++){
                    Rule repeatRule = rules.at(repeatIndex);
                    for (Predicate repeatBodyPred : repeatRule.getBodyPredicates()){
                    }
                    if (bodyPred.getName() == repeatRule.getHeadPredicate().getName()){
                        graph.addEdge(ruleIndex,repeatIndex);
                    }
                }
            }
        }
        return graph;
    }

    void dfs(Node givenNode, Graph& givenGraph, bool forPostOrder){

         givenGraph.getNodesAndDependencies()[givenNode.getNodeID()] = givenNode.setAsVisited();


        for (int adjacentNodeID : givenNode.showOutwardAdjacentNodes()){
            Node adjacentNode = givenGraph.getNodesAndDependencies()[adjacentNodeID];
            if (!adjacentNode.isVisited()){
                dfs(adjacentNode, givenGraph, forPostOrder);
            }
        }
        if (forPostOrder){
            postOrder.push(givenNode.getNodeID());
        }
        else{
            if (!givenNode.isVisited()){
                nodesVisitedForSCC.insert(givenNode.getNodeID());
            }
        }
    }


    void dfsForest(Graph givenGraph){
        givenGraph.setNodesToUnvisited();

        for (const auto& pair : givenGraph.getNodesAndDependencies()){
            if (!pair.second.isVisited()){
                dfs(pair.second, givenGraph, true);
            }
        }
    }

    vector<set<int>> findSCC(Graph originalDependencyGraph){
        vector<set<int>> sccFound;


        while (!postOrder.empty()){
            dfs(originalDependencyGraph.getNodesAndDependencies()[postOrder.top()], originalDependencyGraph, false);
            postOrder.pop();
            if (!nodesVisitedForSCC.empty()){
                sccFound.push_back(nodesVisitedForSCC);
            }
            nodesVisitedForSCC.clear();
        }
        stack<int> emptyNewPostOrder;
        postOrder.swap(emptyNewPostOrder);

        return sccFound;
    }


    void efficientRuleEval(){
        queue<Relation> relationsToJoin;

        stringstream ruleEvalOutput;

        Graph dependencyGraph = makeGraph(givenDatalog.getRules());
        std::cout << "Dependency Graph\n"<< dependencyGraph.toString() << std::endl;
        Graph reversedDependencyGraph = dependencyGraph.createReverseDepGraph();
        dfsForest(reversedDependencyGraph);
        vector<set<int>> allSCCs = findSCC(dependencyGraph);


        for (const set<int>& ind_scc : allSCCs){
            unsigned iter_count = 0;
            bool tupleFound = true;


            ruleEvalOutput << "SCC: ";
            int setPrinter = 0;
            for (int ruleIndex : ind_scc){
                setPrinter++;
                ruleEvalOutput << "R" << ruleIndex;
                if (setPrinter != ind_scc.size()){
                    ruleEvalOutput << ",";
                }
            }
            ruleEvalOutput << "\n";


            while (tupleFound){
                iter_count++;
                tupleFound = false;

                for (int ruleIndex : ind_scc){
                    Rule rule = givenDatalog.getRules().at(ruleIndex);

                    set<int> dependentNodes = dependencyGraph.getNodesAndDependencies()[ruleIndex].showOutwardAdjacentNodes();
                    auto iter = dependentNodes.find(ruleIndex);
                    bool selfDependent = false;
                    if (iter != dependentNodes.end()){
                        selfDependent = true;
                    }
                    ruleEvalOutput << rule.toString() << "\n";

                    for (const Predicate& bodyPred : rule.getBodyPredicates()){
                        relationsToJoin.push(queryEval(bodyPred, false));
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
                        if (ind_scc.size() == 1 && !selfDependent){
                            tupleFound = false;
                        }
                        else{
                            tupleFound = true;
                        }
                    }
                }

            }
            ruleEvalOutput << iter_count << " passes: ";
            setPrinter = 0;
            for (int ruleIndex : ind_scc){
                setPrinter++;
                ruleEvalOutput << "R" << ruleIndex;
                if (setPrinter != ind_scc.size()){
                    ruleEvalOutput << ",";
                }
            }
            ruleEvalOutput << "\n";
        }

        //Project 5 Output Printing
        std::cout << "Rule Evaluation" << std::endl;
        std::cout << ruleEvalOutput.str() << std::endl;
        std::cout << "Query Evaluation" << std::endl;
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


    Relation queryEval(Predicate query, bool outputNeeded){
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
        if (outputNeeded){
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

        return modifiedRelation;
    }

};