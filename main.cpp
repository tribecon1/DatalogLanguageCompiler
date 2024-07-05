#include <iostream>
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
    string filename = argv[1];
    ifstream in_reader;
    in_reader.open(filename);
    stringstream ss;
    ss << in_reader.rdbuf();
    string input = ss.str();
    in_reader.close();


    /*Scanner s = Scanner(input);
    std::vector<Token> tokens = s.scanToken();

    for (const Token& currToken : tokens){
        std::cout << currToken.toString() << std::endl;
    }
    std::cout << "Total Tokens = " << tokens.size() << std::endl;*/

    vector<Token> tokens = {
            Token(Token::ID,"Ned",2),
            //Token(Token::LEFT_PAREN,"(",2),
            Token(Token::ID,"Ted",2),
            Token(Token::COMMA,",",2),
            Token(Token::ID,"Zed",2),
            Token(Token::RIGHT_PAREN,")",2),
    };

    Parser p = Parser(tokens);
    p.scheme();

    return 0;
}
