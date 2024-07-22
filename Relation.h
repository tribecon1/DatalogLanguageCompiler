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


    static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
                         const Tuple& leftTuple, const Tuple& rightTuple) {
        //unordered_map<string, vector<int>> sharedColumnsAndIndexes;

        for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
            const string& leftName = leftScheme.at(leftIndex);
            const string& leftValue = leftTuple.at(leftIndex);
            std::cout << "left name: " << leftName << " value: " << leftValue << std::endl;
            for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
                const string& rightName = rightScheme.at(rightIndex);
                const string& rightValue = rightTuple.at(rightIndex);
                std::cout << "right name: " << rightName << " value: " << rightValue << std::endl;
                if (rightName == leftName && rightValue != leftValue){
                    return false;
                }
                //if you were to modify this to check if values DID match, the following code could work:
                //sharedColumnsAndIndexes.insert({leftScheme.at(left_column_ind), vector<int>{left_column_ind, right_column_ind}});
            }
        }
        return true;
    }

    Scheme schemeJoiner(const Scheme& leftScheme, const Scheme& rightScheme){
        vector<string> joinedSchemeColumns;
        set<string> unique;
        for (const string& leftColumn : leftScheme){
            for (const string& rightColumn : rightScheme){
                if (leftColumn == rightColumn && unique.insert(leftColumn).second){
                    joinedSchemeColumns.push_back(leftColumn);
                }
            }
        }
        return Scheme(joinedSchemeColumns);
    }


    Relation join(const Relation& right) {
        const Relation& left = *this;
        Relation result(left.name, schemeJoiner(left.column_headers, right.column_headers));
        std::cout << result.toString() << std::endl;
        // add code to complete the join operation
        for (const Tuple& leftTuple : left.tuples){
            std::cout << leftTuple.toString(left.column_headers) << std::endl;
            for (const Tuple& rightTuple : right.tuples){
                std::cout << rightTuple.toString(right.column_headers) << std::endl;
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