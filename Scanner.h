#pragma once

#include "Token.h"
#include <cctype>

#include <string>
using std::string;
#include <stack> //for strings checking if its unfinished
#include <utility>
#include <vector>

#include <iostream>

class Scanner {

private:
    string input;
    std::stack<char> string_checker;
    std::vector<Token> TotalTokens;
    int line = 1;

public:
    explicit Scanner(string  input): input(std::move(input)){}


    void alnumHandler(){
        stringstream built_string;
        string potential_type;

        for (char & char_iter : input){
            if (isalnum(char_iter)){
                built_string << char_iter;
            }
            else{
                break;
            }
        }

        potential_type = built_string.str();
        input = input.erase(0, potential_type.size()-1);

        if (potential_type == "Schemes"){
            TotalTokens.emplace_back(Token::SCHEMES, "Schemes", line);
        }
        else if (potential_type == "Facts"){
            TotalTokens.emplace_back(Token::FACTS, "Facts", line);
        }
        else if (potential_type == "Rules"){
            TotalTokens.emplace_back(Token::RULES, "Rules", line);
        }
        else if (potential_type == "Queries"){
            TotalTokens.emplace_back(Token::QUERIES, "Queries", line);
        }
        else{
            TotalTokens.emplace_back(Token::ID, potential_type, line);
        }
    }

    void stringHandler(){
        int string_start = line;
        stringstream building_string;
        building_string << input.at(0);
        input = input.erase(0, 1); //to get first ' quote

        while (!input.empty()){
            while (!input.empty() && input.at(0) != '\''){
                if(input.at(0) == '\n'){
                    line++;
                }
                building_string << input.at(0);
                input = input.erase(0, 1);
            }
            if (!input.empty()){
                building_string << input.at(0); //to get last single quote
                TotalTokens.emplace_back(Token::STRING, building_string.str(), string_start);
                return;
            }
        }
        TotalTokens.emplace_back(Token::UNDEFINED, building_string.str(), string_start);
    }

    void commentHandler(){
        //int comment_start = line;
        stringstream building_comment;
        building_comment << input.at(0);
        input = input.erase(0, 1); //to get #

        for (char ind_char : input){
            if (ind_char == '\n'){
                line++;
                break;
            }
            else{
                building_comment << ind_char;
            }
        }
        input = input.erase(0, building_comment.str().size()-1);
        //TotalTokens.emplace_back(Token::COMMENT, building_comment.str(), comment_start);
    }


    auto scanToken() {
        Token::TokenType type;
        string value;

        while (!input.empty()){

            if (isspace(input.at(0))){
                if (input.at(0) == '\n'){ //clears any leading whitespaces
                    line++;
                }
            }
            else if (isalpha(input.at(0))){
                alnumHandler();
            }
            else if (input.at(0) == '\''){
                stringHandler();
            }
            else if (input.at(0) == '#'){
                commentHandler();
            }
            else{
                switch(input.at(0)){
                    case(','):
                        type = Token::COMMA;
                        value = ',';
                        break;
                    case('.'):
                        type = Token::PERIOD;
                        value = '.';
                        break;
                    case('?'):
                        type = Token::Q_MARK;
                        value = '?';
                        break;
                    case('('):
                        type = Token::LEFT_PAREN;
                        value = '(';
                        break;
                    case(')'):
                        type = Token::RIGHT_PAREN;
                        value = ')';
                        break;
                    case(':'):
                        if(input.size() > 1 && input.at(1) == '-'){
                            type = Token::COLON_DASH;
                            value = ":-";
                            input = input.erase(0,1);
                        }
                        else{
                            type = Token::COLON;
                            value = ':';
                        }
                        break;
                    case('*'):
                        type = Token::MULTIPLY;
                        value = '*';
                        break;
                    case('+'):
                        type = Token::ADD;
                        value = '+';
                        break;
                    default:
                        type = Token::UNDEFINED;
                        value = input.at(0);
                }
                TotalTokens.emplace_back(type, value, line);
            }

            input = input.erase(0,1);
        }

        TotalTokens.emplace_back(Token::TokenType::END, "", line);
        return TotalTokens;
    }

};