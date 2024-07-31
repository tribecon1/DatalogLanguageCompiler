#pragma once

#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <set>
using std::set;


class Node {

private:
    set<int> adjacentNodeIDs;
    bool visited = false;

public:

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    void setAsVisited(){
        visited = true;
    }

    set<int> showOutwardAdjacentNodes(){
        return adjacentNodeIDs;
    }



    [[nodiscard]] string toString() const{
        stringstream out;
        for (int nodeRef : adjacentNodeIDs){
            out << "R" << nodeRef << ",";
        }
        string output = out.str();
        if (!output.empty()){
            output.pop_back();
        }
        return output;
    }

};