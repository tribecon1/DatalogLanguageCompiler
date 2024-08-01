#pragma once

#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <set>
using std::set;


class Node {

private:
    int nodeID;
    set<int> adjacentNodeIDs;
    bool visited = false;

public:
    explicit Node() {}
    explicit Node(int ruleIndex) : nodeID(ruleIndex){ }

    int getNodeID(){
        return nodeID;
    }

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    Node setAsVisited(){
        Node visitedNode = *this;
        visitedNode.visited = true;
        return visitedNode;
    }

    void clearVisit(){
        visited = false;
    }

    [[nodiscard]] bool isVisited() const{
        return visited;
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