#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <type_traits>
#include "../include/lexer.h"

void addToken(TokenType type, TokenValue value, std::vector<Token> *tokens, int line, int column) {
    tokens->push_back({type, value, line, column});
}

//NOTE: Use struct for different states passed here
bool isBoundary(char c, bool *is_newline, bool *is_tab, bool *is_space) {
    if (c == '\n') {
        *is_newline = true;
        return true;
    }

    else if (c == '\t') {
        *is_tab = true;
        return true;
    }

    else if (c == ' ') {
        *is_space = true;
        return true;
    }

    return (std::isspace(static_cast<unsigned char>(c)) ||
            c == '+' || c == '-' ||
            c == '/' || c == '*' ||
            c == '=' || c == ';' ||
            c == '[' || c == ']' ||
            c == '{' || c == '}' ||
            c == '(' || c == ')' ||
            c == ',' || c == '%');
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
        case TokenType::IDENTIFIER:    return "ID"; 

        //keywords
        case TokenType::INT:     return "INT_TYPE";
        case TokenType::FLOAT:     return "FLOAT_TYPE";
        case TokenType::VOID:     return "VOID";
        case TokenType::KERNEL:     return "KER";
        case TokenType::SHARED:     return "SHARED";
        case TokenType::GLOBAL:     return "GLOBAL";

        //numeric literals
        case TokenType::INTEGER:    return "INT";
        case TokenType::DOUBLE:    return "DBL";

        case TokenType::PLUS:    return "PLUS";
        case TokenType::MINUS:    return "MINUS";
        case TokenType::EQUAL:    return "EQ";
        case TokenType::MULTIPLY:    return "MULTIPLY";
        case TokenType::DIVIDE:    return "DIV";
        case TokenType::MODULO:    return "MOD";

        case TokenType::LEFT_PAREN:    return "LPAREN";
        case TokenType::RIGHT_PAREN:    return "RPAREN";
        case TokenType::LEFT_BRACE:    return "LBRACE";
        case TokenType::RIGHT_BRACE:    return "RBRACE";
        case TokenType::LEFT_BRACKET:    return "LBRACKET";
        case TokenType::RIGHT_BRACKET:    return "RBRACKET";
        case TokenType::COMMA:    return "COMMA";
        case TokenType::SEMI_COLON:    return "SCOLON";

        case TokenType::ENDOFFILE:    return "EOF"; 
        case TokenType::ERROR:     return "ERR";
    }
    return "?";
}

void printList(std::vector<Token> tokenList) {
    std::vector<Token> error_list;
    for (const auto& token : tokenList) {
        std::cout << token.line << ":"
            << token.column  << " \t"
            << to_string(token.type) << ": "
            << token.value << "\n";
        if (token.type == TokenType::ERROR) {
            error_list.push_back(token);
        }
    }

    std::cout << "\n";
    for (const auto& error : error_list) {
        std::cout << "<FILENAME>:" 
            << error.line << ":"
            << error.column << ": "
            << "error: "
            << error.value;
    }
    std::cout << "\n";
}

std::vector<Token> process_input(const std::string& inputString) {

    std::unordered_map<std::string, TokenType> keywords;
    keywords["kernel"] = TokenType::KERNEL;
    keywords["int"] = TokenType::INT;
    keywords["float"] = TokenType::FLOAT;
    keywords["void"] = TokenType::VOID;
    keywords["shared"] = TokenType::SHARED;
    keywords["global"] = TokenType::GLOBAL;

    std::vector<Token> tokenList;
    bool is_double = false;
    bool is_error = false;
    bool is_newline = false;
    bool is_tab = false;
    bool is_space = false;
    size_t i = 0;
    size_t size_str = inputString.length();
    int line = 1;
    int column = 1;
    int char_count = 0;
    while (i < size_str) {

        char c = inputString[i];
        switch(c) {
            //Add all special chars here

            case '+': 
                addToken(TokenType::PLUS, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case '-': 
                addToken(TokenType::MINUS, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case '=': 
                addToken(TokenType::EQUAL, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case '*': 
                addToken(TokenType::MULTIPLY, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case '/': 
                addToken(TokenType::DIVIDE, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case '%': 
                addToken(TokenType::MODULO, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case '(': 
                addToken(TokenType::LEFT_PAREN, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case ')': 
                addToken(TokenType::RIGHT_PAREN, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case '{': 
                addToken(TokenType::LEFT_BRACE, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case '}': 
                addToken(TokenType::RIGHT_BRACE, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case '[': 
                addToken(TokenType::LEFT_BRACKET, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case ']': 
                addToken(TokenType::RIGHT_BRACKET, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case ',': 
                addToken(TokenType::COMMA, c, &tokenList, line, column);
                i++;
                column++;
                break;

            case ';': 
                addToken(TokenType::SEMI_COLON, c, &tokenList, line, column);
                i++;
                column++;
                break;
            
            default:
            if (isAlpha(c)) {
                std::string identifier;
                identifier += c;
                char_count++;

                while (++i < size_str && isAlphaNumeric(inputString[i])) {
                    char new_char = inputString[i];
                    identifier += new_char;
                    char_count++;
                }

                if(auto search = keywords.find(identifier);
                        search != keywords.end()) {
                    addToken(search->second, search->first, &tokenList, line, column);
                    column += char_count;
                    char_count = 0;
                }

                else {
                    addToken(TokenType::IDENTIFIER, identifier, &tokenList, line, column);
                    column += char_count;
                    char_count = 0;
                }

                break;
            }
            else if (isDigit(c)) {
                std::string number;
                std::string error_type;
                number += c;
                char_count++;

                while (++i < size_str) {

                    if (inputString[i] == '.') {
                        if (!is_double) {
                            is_double = true;
                            number += inputString[i];
                            char_count++;
                            if ((i + 1) < size_str && isDigit(inputString[i+1])) {
                                continue;
                            }
                            else {
                                //dot at end of number
                                error_type = "Unexpected character '.' ";
                                is_error = true;
                            }
                        }
                        else {
                            number += inputString[i];
                            char_count++;
                            is_error = true;
                            error_type = "Multiple '.' in a single number ";
                            //two dots in same number
                        }
                    }

                    else if (isDigit(inputString[i])) {
                        number += inputString[i];
                        char_count++;
                    }

//NOTE: Potentially remove boundary handling from here?

                    else if (isBoundary(inputString[i], &is_newline, &is_tab, &is_space)) {
                        if (is_newline) {
                            is_newline = false;
                            line++;
                            column = 1;
                        }
                        else if (is_tab) {
                            is_tab = false;
                            char_count += 4;
                        }
                        else if (is_space) {
                            is_space = false;
                            char_count += 1;
                        }
                        break;
                    }

                    else {
                        is_error = true;
                        error_type = "Unexpected character '", std::string(1, inputString[i]) + "' ";
                        number += inputString[i];
                        char_count++;
                    }
                }

                if (is_error) {
                    number.insert(0, error_type);
                    addToken(TokenType::ERROR, number, &tokenList, line, column);
                    column += char_count;
                    char_count = 0;
                    is_error = false;
                    is_double = false;
                }

                else if (is_double) {
                    try {
                        addToken(TokenType::DOUBLE, std::stod(number), &tokenList, line, column);
                        column += char_count;
                        char_count = 0;
                    }
                    catch (...) {
                        error_type = "Failed convertion to int ";
                        number.insert(0, error_type);
                        addToken(TokenType::ERROR, number, &tokenList, line, column);
                        column += char_count;
                        char_count = 0;
                    }
                    is_double = false;
                }

                else {
                    try {
                        addToken(TokenType::INTEGER, std::stoi(number), &tokenList, line, column);
                        column += char_count;
                        char_count = 0;
                    }
                    catch (...) {
                        error_type = "Failed convertion to int ";
                        number.insert(0, error_type);
                        addToken(TokenType::ERROR, number, &tokenList, line, column);
                        column += char_count;
                        char_count = 0;
                    }
                }

            }
            else {
                if (isBoundary(inputString[i], &is_newline, &is_tab, &is_space)) {
                    i++;
                    if (is_newline) {
                        is_newline = false;
                        line++;
                        column = 1;
                    }
                    else if (is_tab) {
                        is_tab = false;
                        column += 4;
                    }
                    else if (is_space) {
                        is_space = false;
                        column += 1;
                    }
                    else {
                        char_count++;
                    }
                }
                //If start of token is a dot
                else if (c == '.') {
                    std::string dot_error;
                    while((i < size_str) &&
                            !(isBoundary(inputString[i], &is_newline, &is_tab, &is_space))) {
                        dot_error += inputString[i];
                        i++;
                        char_count++;
                    }
                    if (is_newline) {
                        is_newline = false;
                        line++;
                        column = 1;
                    }
                    else if (is_tab) {
                        is_tab = false;
                        char_count += 4;
                    }
                    else if (is_space) {
                        is_space = false;
                        char_count += 1;
                    }
                    else {
                        char_count++;
                    }
                    std::string error_type = "No support for tokens starting with '.'";
                    dot_error.insert(0, error_type);
                    addToken(TokenType::ERROR, dot_error, &tokenList, line, column);
                    column += char_count;
                    char_count = 0;
                    is_error = false;
                }
                else {
                    std::string error_type = "Unexpected character '" + std::string(1, inputString[i]) + "' ";
                    addToken(TokenType::ERROR, error_type, &tokenList, line, column);
                    column += char_count;
                    char_count = 0;
                    i++;
                    column++;
                }
            }
        }
    }
    addToken(TokenType::ENDOFFILE, std::monostate{}, &tokenList, line, column);
    return tokenList;
}
