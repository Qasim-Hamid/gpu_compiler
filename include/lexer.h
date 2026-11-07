#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <type_traits>

enum class TokenType {
    IDENTIFIER,
    
    //Keywords
    INT, FLOAT, VOID,
    KERNEL, SHARED, GLOBAL,

    //Numerical literals
    INTEGER, DOUBLE,

    //OPERATORS
    PLUS, MINUS, EQUAL, MULTIPLY, DIVIDE, MODULO,

    //PUNCTUTATION
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, 
    LEFT_BRACKET, RIGHT_BRACKET,  COMMA, SEMI_COLON,

    //Misc
    ENDOFFILE, ERROR
};

using TokenValue = std::variant<std::monostate, int, double, std::string, char>;

struct Token {
    TokenType type;
    TokenValue value;
    int line;
    int column;
};

void addToken(TokenType type, TokenValue value, std::vector<Token> *tokens);
bool isDigit(char c);
bool isAlpha(char c);
bool isAlphaNumeric(char c);
std::ostream& operator<<(std::ostream& os, const TokenValue& val);
const char* to_string(TokenType type);
void printList(std::vector<Token> tokenList);
std::vector<Token> process_input(const std::string& inputString);
