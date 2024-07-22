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

    //Database modifiedDatabase = interpreter.interpret();
/*
    Scheme scheme1( { "A", "B" } );
    Scheme scheme2( { "B", "C" } );

    Tuple tuple1( {"'1'", "'2'"} );
    Tuple tuple2( {"'3'", "'4'"} );

    Scheme scheme3( { "X", "Y" } );
    Scheme scheme4( { "X", "Y", "Z" } );

    Tuple tuple3( {"'1'", "'4'"} );
    Tuple tuple4( {"'1'", "'2'", "'4'"} );

    std::cout << Relation::joinable(scheme1, scheme2, tuple1, tuple2) << std::endl;
    std::cout << Relation::joinable(scheme2, scheme3, tuple1, tuple2) << std::endl;
    std::cout << Relation::joinable(scheme3, scheme4, tuple1, tuple4) << std::endl;
    std::cout << Relation::joinable(scheme3, scheme4, tuple3, tuple4) << std::endl;
*/

    Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> studentValues[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));


    Relation courseRelation("courses", Scheme( {"ID", "Course"} ));

    vector<string> courseValues[] = {
            {"'42'", "'CS 100'"},
            {"'32'", "'CS 232'"},
    };

    for (auto& value : courseValues){
        courseRelation.addTuple(Tuple(value));
    }

    studentRelation.join(courseRelation);
    return 0;
}
