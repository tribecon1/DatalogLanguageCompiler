#pragma once

#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <unordered_map>

class Token {

public:
    enum TokenType {
        COMMA,
        PERIOD,
        Q_MARK,
        LEFT_PAREN,
        RIGHT_PAREN,
        COLON,
        COLON_DASH,
        MULTIPLY,
        ADD,
        SCHEMES,
        FACTS,
        RULES,
        QUERIES,
        ID,
        STRING,
        COMMENT,
        UNDEFINED,
        END
    };

    const std::unordered_map<TokenType, std::string> TokenTypeIndexed = {
            {TokenType::COMMA, "COMMA"},
            {TokenType::PERIOD, "PERIOD"},
            {TokenType::Q_MARK, "Q_MARK"},
            {TokenType::LEFT_PAREN, "LEFT_PAREN"},
            {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
            {TokenType::COLON, "COLON"},
            {TokenType::COLON_DASH, "COLON_DASH"},
            {TokenType::MULTIPLY, "MULTIPLY"},
            {TokenType::ADD, "ADD"},
            {TokenType::SCHEMES, "SCHEMES"},
            {TokenType::FACTS, "FACTS"},
            {TokenType::RULES, "RULES"},
            {TokenType::QUERIES, "QUERIES"},
            {TokenType::ID, "ID"},
            {TokenType::STRING, "STRING"},
            {TokenType::COMMENT, "COMMENT"},
            {TokenType::UNDEFINED, "UNDEFINED"},
            {TokenType::END, "END"}
    };

    Token(TokenType givenType, string givenValue, int givenNum) : type(givenType), value(givenValue), lineNum(givenNum) {}



    [[nodiscard]] string typeName(TokenType TypeToFind) const{
        auto iter = TokenTypeIndexed.find(TypeToFind);
        if (iter != TokenTypeIndexed.end()){
            return iter->second;
        }
        else{
            return "Unknown Token Type";
        }
    }

    [[nodiscard]] TokenType getType() const{
        return this->type;
    }

    [[nodiscard]] string getValue() const{
        return this->value;
    }

    [[nodiscard]] string toString() const{
        stringstream out_string;
        out_string << "(" << typeName(type) << "," << "\"" << value << "\"" << "," << lineNum << ")";
        return out_string.str();
    }


private:
    TokenType type;
    string value;
    int lineNum;

};