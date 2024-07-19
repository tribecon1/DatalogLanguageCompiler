#include <iostream>
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "ParserObjects.h"
#include "Scheme.h"
#include "Tuple.h"
#include "Relation.h"
#include "Interpreter.h"

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

    return 0;
}
