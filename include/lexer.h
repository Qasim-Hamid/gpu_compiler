#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <type_traits>

enum class TokenType {
    IDENTIFIER,
    // INTEGER,
    // DOUBLE,
    ENDOFFILE,
    KERNEL,
    ERROR
};

using TokenValue = std::variant<std::monostate, int, double, std::string, char>;

struct Token {
    TokenType type;
    TokenValue value;
};

void addToken(TokenType type, TokenValue value, std::vector<Token> *tokens);
bool isDigit(char c);
bool isAlpha(char c);
bool isAlphaNumeric(char c);
std::ostream& operator<<(std::ostream& os, const TokenValue& val);
const char* to_string(TokenType type);
void printList(std::vector<Token> tokenList);
std::vector<Token> process_input(const std::string& inputString);
