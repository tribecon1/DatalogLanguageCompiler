#pragma once


#include "Node.h"
#include <map>
using std::map;

class Graph {

private:

    map<int,Node> nodes;

public:

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node();
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }


    [[nodiscard]] string toString() const{
        stringstream out;

        for (auto& pair: nodes) {
            out << "R" << pair.first << ":";
            out << pair.second.toString() << "\n";
        }

        return out.str();
    }


};