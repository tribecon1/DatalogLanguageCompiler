#include <iostream>

#include<vector>
using std::vector;

#include "Token.h"
#include "ParserObjects.h"


class Parser {

private:
    std::vector<Token> tokens;
    int currTokenIndex = 0;
    Datalog currDatalog;


public:
    explicit Parser(std::vector<Token>& scanner_tokens) : tokens(scanner_tokens) {}

    //helper functions
    [[nodiscard]] Token::TokenType tokenType() const {
        return tokens.at(currTokenIndex).getType();
    }

    [[nodiscard]] Token getCurrToken() const {
        return tokens.at(currTokenIndex);
    }

    void advanceToken() {
        if (!tokens.empty() && currTokenIndex != tokens.size()){
            currTokenIndex++;
        }
    }

    static void throwError() {
        throw std::runtime_error("Error! Improper Syntax");
    }

    void match(Token::TokenType input_token) {
        if (tokenType() == input_token){
            advanceToken();
        }
        else{
            throwError();
        }
    }

    //ind. grammar rule functions

    //recursive list methods, all have lambda because could go on for indefinite time
    void schemeList(){
        if (tokenType() == Token::ID){
            currDatalog.addToSchemes(scheme());
            schemeList();
        }
    }

    void factList(){
        if (tokenType() == Token::ID){
            currDatalog.addToFacts(fact());
            factList();
        }
    }

    void queryList(){
        if (tokenType() == Token::ID){
            currDatalog.addToQueries(query());
            queryList();
        }
    }

    void ruleList(){
        if (tokenType() == Token::ID){
            currDatalog.addToRules(rule());
            ruleList();
        }
    }


    void idList(std::vector<Parameter>& toBeReturned) {
        if (tokenType() == Token::COMMA) {
            match(Token::COMMA);
            toBeReturned.emplace_back(parameterCreator());
            match(Token::ID);
            idList(toBeReturned);
        }
        //else == lambda
    }

    void stringList(std::vector<Parameter>& toBeReturned) {
        if (tokenType() == Token::COMMA) {
            match(Token::COMMA);
            toBeReturned.emplace_back(parameterCreator());
            match(Token::STRING);
            stringList(toBeReturned);
        }
        //else == lambda
    }

    void parameterList(std::vector<Parameter>& toBeReturned){
        if (tokenType() == Token::COMMA){
            match(Token::COMMA);
            toBeReturned.push_back(parameterCreator());
            if (tokenType() == Token::ID){
                match(Token::ID);
            }
            else{
                match(Token::STRING); //can only other be a string since parameterCreator() checks for that
            }
            parameterList(toBeReturned);
        }
        //else == lambda
    }

    void predicateList(std::vector<Predicate>& toBeReturned){ //this method ONLY applies to a Rule obj.
        if (tokenType() == Token::COMMA){
            match(Token::COMMA);
            toBeReturned.emplace_back(predicateCreator(Token::RULES));
            predicateList(toBeReturned);
        }
        //else == lambda
    }


    //object methods
    [[nodiscard]] Parameter parameterCreator() const{ //makes sure the only tokens it makes a parameter out of is EITHER id OR string
        if (tokenType() == Token::ID){
            Parameter new_param = Parameter(getCurrToken());
            return new_param;
        }
        else if (tokenType() == Token::STRING){
            Parameter new_param = Parameter(getCurrToken());
            return new_param;
        }
        else{
            throwError();
        }
    }


    Predicate predicateCreator(Token::TokenType factschemequeryruleSwitch){
        if (tokenType() == Token::ID){
            string predicate_name = getCurrToken().getValue();
            match(Token::ID);
            match(Token::LEFT_PAREN);
            Predicate new_predicate = Predicate(predicate_name); //could be fact, scheme, or query
            if (tokenType() == Token::ID && factschemequeryruleSwitch == Token::SCHEMES){
                std::vector<Parameter> idParameterList;
                new_predicate.addParameter(parameterCreator());
                match(Token::ID);
                idList(idParameterList);
                new_predicate.addParameter(idParameterList);
            }
            else if (tokenType() == Token::STRING && factschemequeryruleSwitch == Token::FACTS){
                std::vector<Parameter> stringParameterList;
                new_predicate.addParameter(parameterCreator());
                match(Token::STRING);
                stringList(stringParameterList);
                new_predicate.addParameter(stringParameterList);
            }
            else if (factschemequeryruleSwitch == Token::QUERIES || factschemequeryruleSwitch == Token::RULES){ //can be a mix of ID and String tokens
                std::vector<Parameter> mixedParameterList;
                Parameter firstParameter = parameterCreator();
                new_predicate.addParameter(firstParameter);
                if(firstParameter.isTypeID()){
                    match(Token::ID);
                }
                else{
                    match(Token::STRING);
                }
                parameterList(mixedParameterList);
                new_predicate.addParameter(mixedParameterList);
            }
            else{
                throwError();
            }
            match(Token::RIGHT_PAREN);
            return new_predicate;
        }
        else{
            throwError();
        }
    }


    Predicate scheme() {
        return predicateCreator(Token::SCHEMES);
    }

    Predicate fact() {
        Predicate new_fact = predicateCreator(Token::FACTS);
        match(Token::PERIOD);
        return new_fact;
    }

    Rule rule(){
        Predicate ruleHead = predicateCreator(Token::SCHEMES);
        std::vector<Predicate> ruleBody;
        Rule newRule = Rule(ruleHead);
        match(Token::COLON_DASH);
        newRule.addPredicates(predicateCreator(Token::RULES));
        predicateList(ruleBody);
        newRule.addPredicates(ruleBody);
        match(Token::PERIOD);
        return newRule;
    }

    Predicate query() {
        Predicate new_query = predicateCreator(Token::QUERIES);
        match(Token::Q_MARK);
        return new_query;
    }

    Datalog datalogParser(){
        try{
            while(tokenType() != Token::END){
                match(Token::SCHEMES);
                match(Token::COLON);
                schemeList();
                if (currDatalog.getSchemes().empty()){
                    throwError();
                }
                match(Token::FACTS);
                match(Token::COLON);
                factList();
                match(Token::RULES);
                match(Token::COLON);
                ruleList();
                match(Token::QUERIES);
                match(Token::COLON);
                queryList();
            }
            if (currDatalog.getQueries().empty()){
                throwError();
            }
            if (!currDatalog.getFacts().empty()){
                for (Predicate currFact : currDatalog.getFacts()){
                    for (const Parameter& currParameter : currFact.getParameters()){
                        currDatalog.addToDomain(currParameter.toString());
                    }
                }
            }
            //std::cout << currDatalog.toString() << std::endl;
        }
        catch(const std::runtime_error& e){
            std::cout << "Failure!" << std::endl;
            std::cout << "  " + getCurrToken().toString() << std::endl;
        }
        return currDatalog;
    }


};



