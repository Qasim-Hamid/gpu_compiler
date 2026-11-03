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
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
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
        case TokenType::INTEGER:    return "INT";
        case TokenType::DOUBLE:    return "DBL";
        case TokenType::PLUS:    return "PLUS";
        case TokenType::MINUS:    return "MINUS";
        case TokenType::EQUAL:    return "EQ";
        case TokenType::MULTIPLY:    return "MULTIPLY";
        case TokenType::DIVIDE:    return "DIV";
        case TokenType::MODULO:    return "MOD";
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
    bool is_double = false;
    bool is_error = false;
    int i = 0;
    int size_str = inputString.length();
    while (i < size_str) {

        char c = inputString[i];
        switch(c) {
            //Add all special chars here

            case '+': 
                addToken(TokenType::PLUS, c, &tokenList);
                i++;
                break;

            case '-': 
                addToken(TokenType::MINUS, c, &tokenList);
                i++;
                break;

            case '=': 
                addToken(TokenType::EQUAL, c, &tokenList);
                i++;
                break;

            case '*': 
                addToken(TokenType::MULTIPLY, c, &tokenList);
                i++;
                break;

            case '/': 
                addToken(TokenType::DIVIDE, c, &tokenList);
                i++;
                break;

            case '%': 
                addToken(TokenType::MODULO, c, &tokenList);
                i++;
                break;
            
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
                    addToken(search->second, search->first, &tokenList);
                }

                else {
                    addToken(TokenType::IDENTIFIER, identifier, &tokenList);
                }

                break;
            }
            else if (isDigit(c)) {
                std::string number;
                //dot before num
                if (is_error) {
                    number += '.';
                }
                number += c;

                while (++i < size_str && (isDigit(inputString[i]) || inputString[i] == '.')) {

                    if (inputString[i] == '.') {
                        if (!is_double) {
                            is_double = true;
                            number += inputString[i];
                            if ((i + 1) < size_str && isDigit(inputString[i+1])) {
                                continue;
                            }
                            else {
                                //dot at end of number
                                is_error = true;
                            }
                        }
                        else {
                            number += inputString[i];
                            is_error = true;
                            //two dots in same number
                        }
                    }
                    else {
                        number += inputString[i];
                    }
                }

                if (is_error) {
                    addToken(TokenType::ERROR, number, &tokenList);
                    is_error = false;
                    is_double = false;
                }
                else if (is_double) {
                    addToken(TokenType::DOUBLE, std::stod(number), &tokenList);
                    is_double = false;
                }
                else {
                    addToken(TokenType::INTEGER, std::stoi(number), &tokenList);
                }
            }
            else {
                if (std::isspace(static_cast<unsigned char>(c))) {
                    i++;
                }
                else if (c == '.') {
                    if ((i + 1) < size_str && isDigit(inputString[i+1])) {
                        is_double = true;
                        is_error = true;
                        i++;
                    }
                }
                else {
                    // std::cout << "ERROR\n";
                    addToken(TokenType::ERROR, c, &tokenList);
                    i++;
                }
            }
        }
    }
    addToken(TokenType::ENDOFFILE, std::monostate{}, &tokenList);
    return tokenList;
}
