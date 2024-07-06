#include <iostream>
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "ParserObjects.h"

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

//    for (const Token& currToken : tokens){
//        std::cout << currToken.toString() << std::endl;
//    }
//    std::cout << "Total Tokens = " << tokens.size() << std::endl;

//    for (Token t : tokens){
//        std::cout << t.toString() << ", ";
//    }
//    std::cout << std::endl;

    Parser p = Parser(tokens);
    Datalog newDatalog = p.datalogParser();

    //std::cout << newDatalog.toString() << std::endl;

    return 0;
}
