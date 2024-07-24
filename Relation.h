#pragma once

#include <iostream>
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <utility>
#include <vector>
using std::vector;
#include <unordered_map>
using std::unordered_map;
#include <set>
using std::set;
#include <utility>
using std::pair;

#include "Scheme.h"
#include "Tuple.h"

class Relation{

private:
    string name;
    set<Tuple> tuples;
    Scheme column_headers;

public:

    Relation(const string& name, const Scheme& scheme) : name(name), column_headers(scheme) { }

    [[nodiscard]] string getName(){
        return name;
    }

    [[nodiscard]] Scheme getScheme(){
        return column_headers;
    }

    [[nodiscard]] unsigned getTupleCount(){
        return tuples.size();
    }

    //made boolean to see if it was unique (the tuple that was added to the relation's set of tuples)
    bool addTuple(const Tuple& tuple) {
        //tuples.insert(tuple);
        if (tuple.size() == column_headers.size()){
            auto added = tuples.insert(tuple);
            return added.second;
        }
        else{
            throw std::out_of_range("Error! Tuple length must be length: " + std::to_string(column_headers.size()) + " to match the Scheme length");
        }
    }

    [[nodiscard]] Relation select(int index, const string& value) const{
        Relation result(name, column_headers); //same specified value, same column
        for (const Tuple& tuple : tuples){
            if (tuple.at(index) == value){
                result.addTuple(tuple);
            }
        }
        return result;
    }

    [[nodiscard]] Relation select(const vector<int>& varIndexes) const{ //same unspecified value, diff. columns
        Relation result(name, column_headers);
        for (const Tuple& tuple : tuples){
            bool toBeAdded = true;
            for (int index : varIndexes){
                if (tuple.at(index) != tuple.at(varIndexes.at(0))){
                    toBeAdded = false;
                }
            }
            if (toBeAdded){
                result.addTuple(tuple);
            }
        }
        return result;
    }


    Relation rename(const Scheme& new_scheme){
        Relation renamedRelation = *this;
        renamedRelation.column_headers = new_scheme;
        return renamedRelation;
    }


    Relation project(const std::set<int>& chosen_columns_ind){
        vector<string> modScheme;
        modScheme.reserve(chosen_columns_ind.size());
        for (int col_index : chosen_columns_ind){
            modScheme.push_back(column_headers.at(col_index));
        }
        Relation projectedRelation = Relation(name, Scheme(modScheme));

        for (Tuple tuple : tuples){
            vector<string> modTupleValues;
            modTupleValues.reserve(chosen_columns_ind.size());
            for (int tuple_index : chosen_columns_ind){
                modTupleValues.push_back(tuple.getValues().at(tuple_index));
            }
            projectedRelation.addTuple(Tuple(modTupleValues));
        }

        return projectedRelation;
    }





    static pair<bool,unordered_map<string, int>> joinable(const Scheme& leftScheme, const Scheme& rightScheme,
                         const Tuple& leftTuple, const Tuple& rightTuple) {
        unordered_map<string, int> sharedColumnsAndIndexes;

        for (int leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
            const string& leftName = leftScheme.at(leftIndex);
            const string& leftValue = leftTuple.at(leftIndex);
            for (int rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
                const string& rightName = rightScheme.at(rightIndex);
                const string& rightValue = rightTuple.at(rightIndex);
                if (rightName == leftName){
                    if (rightValue != leftValue){
                        return {false, sharedColumnsAndIndexes};
                    }
                    else{
                        sharedColumnsAndIndexes.insert({leftScheme.at(leftIndex), rightIndex});
                    }
                }
            }
        }
        return {true, sharedColumnsAndIndexes};
    }

    Scheme schemeJoiner(Scheme leftScheme, const Scheme& rightScheme){
        set<string> unique(leftScheme.begin(), leftScheme.end());
        for (const string& rightColumn : rightScheme){
            if (unique.insert(rightColumn).second){
                leftScheme.push_back(rightColumn);
            }
        }
        return {leftScheme};
    }

    Tuple tupleJoiner(const unordered_map<string, int>& sharedColumnsAndIndexes, Tuple leftTuple, Tuple rightTuple){
        vector<string> newTupleVals = leftTuple.getValues();
        vector<string> remainingUniqueVals = rightTuple.getValues();
        for (const auto& colAndIndexes : sharedColumnsAndIndexes){ //for if they have multiple shared columns
            remainingUniqueVals.erase(remainingUniqueVals.begin() + colAndIndexes.second);
        }
        for (const string& remainingVal : remainingUniqueVals){
            newTupleVals.push_back(remainingVal);
        }
        return Tuple(newTupleVals);
    }


    Relation join(const Relation& right) {
        const Relation& left = *this;
        Scheme joinedScheme = schemeJoiner(left.column_headers, right.column_headers);

        Relation result(left.name, joinedScheme);
        for (const Tuple& leftTuple : left.tuples){
            for (const Tuple& rightTuple : right.tuples){
                auto joinChecker = joinable(left.column_headers, right.column_headers, leftTuple, rightTuple);
                if (joinChecker.first){
                    Tuple joinedTuple = tupleJoiner(joinChecker.second, leftTuple, rightTuple);
                    result.addTuple(joinedTuple);
                    //UTILIZE BOOL RETURN VAL OF addTuple LATER FOR EACH RULE
                }
            }
        }

        return result;
    }


    [[nodiscard]] string toString() const {
        stringstream out;
        for (const Tuple& tuple : tuples) {
            std::cout << "  " << tuple.toString(column_headers) << std::endl;
        }
        return out.str();
    }


};