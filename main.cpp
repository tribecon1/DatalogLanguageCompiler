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


    //std::cout << input << std::endl;
    Scanner s = Scanner(input);
    std::vector<Token> tokens = s.scanToken();

//    for (Token t : tokens){
//        std::cout << t.toString() << ", ";
//    }
//    std::cout << std::endl;

    Parser p = Parser(tokens);
    Datalog newDatalog = p.datalogParser();

    /*vector<string> names = { "ID", "Name", "Major" };
    Scheme scheme(names);
    Relation relation("student", scheme);
    vector<string> values[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'32'", "'Bob'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
            {"'16'", "'Jim'", "'EE'"},
    };

    for (auto& value : values) {
        Tuple tuple(value);
        std::cout << tuple.toString(scheme) << std::endl;
        relation.addTuple(tuple);
    }

    std::cout << "relation:" << std::endl;
    std::cout << relation.toString();

    Relation result = relation.select(2, "'CS'");

    std::cout << "select Major='CS' result:" << std::endl;
    std::cout << result.toString();*/

    Database newDatabase;
    Interpreter interpreter = Interpreter(newDatalog, newDatabase);

    Database modifiedDatabase = interpreter.interpret();




//if string, call select 1 (pos, value)
//if id, call select 2 (pos, pos)
    return 0;
}
