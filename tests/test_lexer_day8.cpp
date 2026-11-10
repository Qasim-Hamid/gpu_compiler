#include "test_helpers.h"

using namespace test;

// Day 8 language choices: IF, ELSE, FOR, WHILE are keyword tokens
// whose values are strings; true/false share BOOLEAN with a bool payload.
// Add bool to TokenValue (std::variant) if you have not already.

int main() {
    Suite s("Day 8");

    s.check("all control-flow keywords", "if else for while", {
        At(TokenType::IF, "if", 1, 1),
        At(TokenType::ELSE, "else", 1, 4),
        At(TokenType::FOR, "for", 1, 9),
        At(TokenType::WHILE, "while", 1, 13),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 18)
    });

    s.check("if at EOF", "if", {
        At(TokenType::IF, "if", 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 3)
    });

    s.check("for and while adjacent to punctuation", "for(while)", {
        At(TokenType::FOR, "for", 1, 1),
        At(TokenType::LEFT_PAREN, '(', 1, 4),
        At(TokenType::WHILE, "while", 1, 5),
        At(TokenType::RIGHT_PAREN, ')', 1, 10),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 11)
    });

    s.check("else in a block", "if{}else{}", {
        At(TokenType::IF, "if", 1, 1),
        At(TokenType::LEFT_BRACE, '{', 1, 3),
        At(TokenType::RIGHT_BRACE, '}', 1, 4),
        At(TokenType::ELSE, "else", 1, 5),
        At(TokenType::LEFT_BRACE, '{', 1, 9),
        At(TokenType::RIGHT_BRACE, '}', 1, 10),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 11)
    });

    s.check("boolean true at EOF", "true", {
        At(TokenType::BOOLEAN, TokenValue{std::in_place_type<bool>, true}, 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("boolean false at EOF", "false", {
        At(TokenType::BOOLEAN, false, 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("both boolean values", "true false", {
        At(TokenType::BOOLEAN, true, 1, 1),
        At(TokenType::BOOLEAN, false, 1, 6),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 11)
    });

    s.check("keyword prefixes stay identifiers", "iffy elsewhere", {
        At(TokenType::IDENTIFIER, "iffy", 1, 1),
        At(TokenType::IDENTIFIER, "elsewhere", 1, 6),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 15)
    });

    s.check("loop keyword prefixes stay identifiers", "format while_loop", {
        At(TokenType::IDENTIFIER, "format", 1, 1),
        At(TokenType::IDENTIFIER, "while_loop", 1, 8),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 18)
    });

    s.check("boolean prefixes stay identifiers", "true_value false123", {
        At(TokenType::IDENTIFIER, "true_value", 1, 1),
        At(TokenType::IDENTIFIER, "false123", 1, 12),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 20)
    });

    s.check("keyword suffixes stay identifiers", "myif _else for2", {
        At(TokenType::IDENTIFIER, "myif", 1, 1),
        At(TokenType::IDENTIFIER, "_else", 1, 6),
        At(TokenType::IDENTIFIER, "for2", 1, 12),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 16)
    });

    s.check("if is case-sensitive", "IF If if", {
        At(TokenType::IDENTIFIER, "IF", 1, 1),
        At(TokenType::IDENTIFIER, "If", 1, 4),
        At(TokenType::IF, "if", 1, 7),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 9)
    });

    s.check("boolean literals are case-sensitive", "TRUE True true", {
        At(TokenType::IDENTIFIER, "TRUE", 1, 1),
        At(TokenType::IDENTIFIER, "True", 1, 6),
        At(TokenType::BOOLEAN, true, 1, 11),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 15)
    });

    s.check("mixed-case control keywords stay identifiers", "Else FOR While", {
        At(TokenType::IDENTIFIER, "Else", 1, 1),
        At(TokenType::IDENTIFIER, "FOR", 1, 6),
        At(TokenType::IDENTIFIER, "While", 1, 10),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 15)
    });

    s.check("boolean logical AND", "true&&false", {
        At(TokenType::BOOLEAN, true, 1, 1),
        At(TokenType::AND_AND, "&&", 1, 5),
        At(TokenType::BOOLEAN, false, 1, 7),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 12)
    });

    s.check("boolean negation and OR", "!false||true", {
        At(TokenType::BANG, '!', 1, 1),
        At(TokenType::BOOLEAN, false, 1, 2),
        At(TokenType::OR_OR, "||", 1, 7),
        At(TokenType::BOOLEAN, true, 1, 9),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 13)
    });

    s.check("nested control flow and positions", "if(true){\n    while(false){}\n}", {
        At(TokenType::IF, "if", 1, 1),
        At(TokenType::LEFT_PAREN, '(', 1, 3),
        At(TokenType::BOOLEAN, true, 1, 4),
        At(TokenType::RIGHT_PAREN, ')', 1, 8),
        At(TokenType::LEFT_BRACE, '{', 1, 9),
        At(TokenType::WHILE, "while", 2, 5),
        At(TokenType::LEFT_PAREN, '(', 2, 10),
        At(TokenType::BOOLEAN, false, 2, 11),
        At(TokenType::RIGHT_PAREN, ')', 2, 16),
        At(TokenType::LEFT_BRACE, '{', 2, 17),
        At(TokenType::RIGHT_BRACE, '}', 2, 18),
        At(TokenType::RIGHT_BRACE, '}', 3, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 3, 2)
    });

    s.check("keywords across blank lines", "if\n\n  else", {
        At(TokenType::IF, "if", 1, 1),
        At(TokenType::ELSE, "else", 3, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 3, 7)
    });

    s.check("block comment between keyword and boolean", "if/*condition*/(true)", {
        At(TokenType::IF, "if", 1, 1),
        At(TokenType::LEFT_PAREN, '(', 1, 16),
        At(TokenType::BOOLEAN, true, 1, 17),
        At(TokenType::RIGHT_PAREN, ')', 1, 21),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 22)
    });

    s.check("line comment before loop keyword", "// ignore if else\nwhile(false)", {
        At(TokenType::WHILE, "while", 2, 1),
        At(TokenType::LEFT_PAREN, '(', 2, 6),
        At(TokenType::BOOLEAN, false, 2, 7),
        At(TokenType::RIGHT_PAREN, ')', 2, 12),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 13)
    });

    return s.finish();
}
