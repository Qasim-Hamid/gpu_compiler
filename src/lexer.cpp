#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <type_traits>
#include "../include/lexer.h"

void addToken(TokenType type, TokenValue value, std::vector<Token> *tokens) {
    tokens->push_back({type, value});
}

bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z' ||
            c >= 'A' && c <= 'Z' ||
            c == '_');
}

bool isAlphaNumeric(char c) {
    return (isAlpha(c) || isDigit(c));
}

std::ostream& operator<<(std::ostream& os, const TokenValue& val) {
    std::visit([&os](const auto& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            os << "N/A";
        }
        else {
            os << arg;
        }
    }, val);
    return os;
}

const char* to_string(TokenType type) {
    switch(type) {
        // case TokenType::INTEGER:    return "INT"; 
        // case TokenType::DOUBLE:    return "DBL"; 
        case TokenType::IDENTIFIER:    return "ID"; 
        case TokenType::ENDOFFILE:    return "EOF"; 
        case TokenType::KERNEL:     return "KER";
        case TokenType::ERROR:     return "ERR";
    }
    return "?";
}

void printList(std::vector<Token> tokenList) {
    for (const auto& token : tokenList) {
        std::cout << to_string(token.type) << ": "
            << token.value << "\n";
    }
}

std::vector<Token> process_input(const std::string& inputString) {

    std::unordered_map<std::string, TokenType> keywords;
    keywords["kernel"] = TokenType::KERNEL;
    std::vector<Token> tokenList;

    int i = 0;
    int size_str = inputString.length();
    while (i < size_str) {

        char c = inputString[i];
        switch(c) {
            //Add all special chars here
            default:
            if (isAlpha(c)) {
                std::string identifier;
                identifier += c;

                while (++i < size_str && isAlphaNumeric(inputString[i])) {
                    char new_char = inputString[i];
                    identifier += new_char;
                }

                if(auto search = keywords.find(identifier);
                        search != keywords.end()) {
                    addToken(search->second, identifier, &tokenList);
                }

                else {
                    addToken(TokenType::IDENTIFIER, identifier, &tokenList);
                }

                break;
            }
            else {
                if (std::isspace(static_cast<unsigned char>(c))) {
                    i++;
                }
                else {
                    addToken(TokenType::ERROR, c, &tokenList);
                    i++;
                }
            }
        }
    }
    addToken(TokenType::ENDOFFILE, std::monostate{}, &tokenList);
    return tokenList;
}
