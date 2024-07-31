#pragma once


#include "Node.h"
#include <map>
#include <utility>
using std::map;

class Graph {

private:

    map<int,Node> nodes;

public:

    explicit Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node();
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }


    Graph createReverseDepGraph(){
        Graph reversedGraph = Graph(this->nodes.size());

        for (auto pair : nodes){
            for (int outNodeIdentifier : pair.second.showOutwardAdjacentNodes()){
                reversedGraph.addEdge(outNodeIdentifier, pair.first);
            }
        }
        return reversedGraph;
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