#include <iostream>
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "ParserObjects.h"
#include "Interpreter.h"
#include "Node.h"
#include "Graph.h"

//std::stack<int> Interpreter::postOrder;
//std::vector<int> Interpreter::nodesVisitedForSCC;



int main(int argc, char* argv[]) {
    string filename = argv[1];
    ifstream in_reader;
    in_reader.open(filename);
    stringstream ss;
    ss << in_reader.rdbuf();
    string input = ss.str();
    in_reader.close();


    Scanner s = Scanner(input);
    std::vector<Token> tokens = s.scanToken();


    Parser p = Parser(tokens);
    Datalog newDatalog = p.datalogParser();

    Database newDatabase;
    Interpreter interpreter = Interpreter(newDatalog, newDatabase);

    Database modifiedDatabase = interpreter.interpret();


    /*Node node;
    node.addEdge(4);
    node.addEdge(8);
    node.addEdge(2);
    std::cout << node.toString() << std::endl;

    Graph graph(3);
    graph.addEdge(1,2);
    graph.addEdge(1,0);
    graph.addEdge(0,1);
    graph.addEdge(1,1);
    std::cout << graph.toString();*/

    // predicate names for fake rules
    // first is name for head predicate
    // second is names for body predicates
    pair<string,vector<string>> ruleNames[] = {
            { "A", { "B", "C" } },
            { "B", { "A", "D" } },
            { "B", { "B" } },
            { "E", { "F", "G" } },
            { "E", { "E", "F" } },
    };

    vector<Rule> rules;

    for (auto& rulePair : ruleNames) {
        string headName = rulePair.first;
        Rule rule = Rule(Predicate(headName));
        vector<string> bodyNames = rulePair.second;
        for (auto& bodyName : bodyNames)
            rule.addPredicates(Predicate(bodyName));
        rules.push_back(rule);
    }


//    Graph graph = Interpreter::makeGraph(rules);
//    std::cout << graph.toString() << std::endl;
//    Graph reversedGraph = graph.createReverseDepGraph();
//    std::cout << reversedGraph.toString();
//    Interpreter::dfsForest(reversedGraph);
//    std::cout << "\nPostOrder Stack Order:" << std::endl;
//    stack<int> toPrintPostOrder = Interpreter::postOrder;
//    while (!toPrintPostOrder.empty()){
//        std::cout << "R" << toPrintPostOrder.top() << std::endl;
//        toPrintPostOrder.pop();
//    }
//    vector<vector<int>> sccs = Interpreter::findSCC(graph);
//    std::cout << "\nSCCs Found: " << sccs.size() << std::endl;



    return 0;
}
