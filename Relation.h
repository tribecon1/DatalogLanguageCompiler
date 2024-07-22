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


    void addTuple(const Tuple& tuple) {
        //tuples.insert(tuple);

        if (tuple.size() == column_headers.size()){
            tuples.insert(tuple);
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
            }
        }


        /*for (int left_column_ind = 0; left_column_ind < leftScheme.size(); left_column_ind++){
            for (int right_column_ind = 0; right_column_ind < rightScheme.size(); right_column_ind++){
                if (leftScheme.at(left_column_ind) == rightScheme.at(right_column_ind)){
//                    if (leftTuple.at(left_column_ind) == rightTuple.at(right_column_ind)){
//                        //sharedColumnsAndIndexes.insert({leftScheme.at(left_column_ind), vector<int>{left_column_ind, right_column_ind}});
//                    }
//                    else{
//                        return false;
//                    }
                    if (leftTuple.at(left_column_ind) != rightTuple.at(right_column_ind)){
                        return false;
                    }
                }
            }
        }*/

        return true;
    }


    [[nodiscard]] string toString() const {
        stringstream out;
        for (const Tuple& tuple : tuples) {
            std::cout << "  " << tuple.toString(column_headers) << std::endl;
        }
        return out.str();
    }


};