#include <iostream>
#include <string>
#include <utility>
using std::string;

#include "ParserObjects.h"
#include "Database.h"
#include "Scheme.h"
#include "Tuple.h"


class Interpreter{

private:
    Datalog givenDatalog;
    Database newDatabase;

public:
    Interpreter(Datalog givenDatalog, Database emptyDatabase) : givenDatalog(std::move(givenDatalog)), newDatabase(std::move(emptyDatabase)) {}
};