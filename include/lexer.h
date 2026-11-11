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
    IF, ELSE, FOR, WHILE,

    //BOOL LITERALS
    BOOLEAN,
    
    //Numerical literals
    INTEGER, DOUBLE,

    //OPERATORS
    PLUS, MINUS, EQUAL, MULTIPLY, DIVIDE, MODULO,
    BANG, BANG_EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
    AND, AND_AND, OR_OR,

    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL,
    MOD_EQUAL, PLUS_PLUS, MINUS_MINUS,

    //PUNCTUTATION
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, 
    LEFT_BRACKET, RIGHT_BRACKET,  COMMA, SEMI_COLON,

    //Misc
    ENDOFFILE, ERROR
};

using TokenValue = std::variant<std::monostate, int, double, std::string, char, bool>;

struct Token {
    TokenType type;
    TokenValue value;
    int line;
    int column;
};

void addToken(TokenType type, TokenValue value, std::vector<Token> *tokens, int line, int column);
bool isDigit(char c);
bool isAlpha(char c);
bool isAlphaNumeric(char c);
std::ostream& operator<<(std::ostream& os, const TokenValue& val);
const char* to_string(TokenType type);
void printList(std::vector<Token> tokenList, std::string filename, bool *has_error);
std::vector<Token> process_input(const std::string& inputString);
