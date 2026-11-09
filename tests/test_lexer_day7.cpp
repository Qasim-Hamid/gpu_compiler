#include "test_helpers.h"

using namespace test;

// Day 7: assumes the suggested TokenType names from the Day 7 assignment.
// Rename EQUAL_EQUAL, BANG_EQUAL, LESS, LESS_EQUAL, GREATER,
// GREATER_EQUAL, AND_AND, OR_OR, BANG if your enum uses other names.

int main() {
    Suite s("Day 7");

    s.check("assignment remains one character", "a=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::EQUAL, '=', 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    s.check("equality longest match", "a==b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::EQUAL_EQUAL, "==", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("inequality longest match", "a!=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::BANG_EQUAL, "!=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("adjacent equality operators", "a===b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::EQUAL_EQUAL, "==", 1, 2),
        At(TokenType::EQUAL, '=', 1, 4),
        At(TokenType::IDENTIFIER, "b", 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("less than", "a<b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::LESS, '<', 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    s.check("less than or equal", "a<=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::LESS_EQUAL, "<=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("greater than", "a>b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::GREATER, '>', 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    s.check("greater than or equal", "a>=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::GREATER_EQUAL, ">=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("comparison adjacent to punctuation", "(a<=b)", {
        At(TokenType::LEFT_PAREN, '(', 1, 1),
        At(TokenType::IDENTIFIER, "a", 1, 2),
        At(TokenType::LESS_EQUAL, "<=", 1, 3),
        At(TokenType::IDENTIFIER, "b", 1, 5),
        At(TokenType::RIGHT_PAREN, ')', 1, 6),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 7)
    });

    s.check("consecutive relational operators", "a<=>b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::LESS_EQUAL, "<=", 1, 2),
        At(TokenType::GREATER, '>', 1, 4),
        At(TokenType::IDENTIFIER, "b", 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("logical and", "a&&b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::AND_AND, "&&", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("logical or", "a||b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::OR_OR, "||", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("logical not", "!a", {
        At(TokenType::BANG, '!', 1, 1),
        At(TokenType::IDENTIFIER, "a", 1, 2),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 3)
    });

    s.check("not versus not equal", "!a!=b", {
        At(TokenType::BANG, '!', 1, 1),
        At(TokenType::IDENTIFIER, "a", 1, 2),
        At(TokenType::BANG_EQUAL, "!=", 1, 3),
        At(TokenType::IDENTIFIER, "b", 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("mixed logical operators", "a&&b||!c", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::AND_AND, "&&", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::OR_OR, "||", 1, 5),
        At(TokenType::BANG, '!', 1, 7),
        At(TokenType::IDENTIFIER, "c", 1, 8),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 9)
    });

    s.check("single ampersand", "a&b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::AND, 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    s.check("single pipe is error", "a|b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::ERROR, 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    s.check("triple ampersand recovers", "a&&&b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::AND_AND, "&&", 1, 2),
        At(TokenType::AND, 1, 4),
        At(TokenType::IDENTIFIER, "b", 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("triple pipe recovers", "a|||b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::OR_OR, "||", 1, 2),
        At(TokenType::ERROR, 1, 4),
        At(TokenType::IDENTIFIER, "b", 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("integer before less-than", "123<456", {
        At(TokenType::INTEGER, 123, 1, 1),
        At(TokenType::LESS, '<', 1, 4),
        At(TokenType::INTEGER, 456, 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 8)
    });

    s.check("double before greater-or-equal", "3.14>=2", {
        At(TokenType::DOUBLE, 3.14, 1, 1),
        At(TokenType::GREATER_EQUAL, ">=", 1, 5),
        At(TokenType::INTEGER, 2, 1, 7),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 8)
    });

    s.check("integers around equality", "42==42", {
        At(TokenType::INTEGER, 42, 1, 1),
        At(TokenType::EQUAL_EQUAL, "==", 1, 3),
        At(TokenType::INTEGER, 42, 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 7)
    });

    s.check("number before logical and", "1&&0", {
        At(TokenType::INTEGER, 1, 1, 1),
        At(TokenType::AND_AND, "&&", 1, 2),
        At(TokenType::INTEGER, 0, 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("block comment before equality", "a/*comment*/==b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::EQUAL_EQUAL, "==", 1, 13),
        At(TokenType::IDENTIFIER, "b", 1, 15),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 16)
    });

    s.check("line comment before comparison", "x//comment\n>=y", {
        At(TokenType::IDENTIFIER, "x", 1, 1),
        At(TokenType::GREATER_EQUAL, ">=", 2, 1),
        At(TokenType::IDENTIFIER, "y", 2, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 4)
    });

    s.check("operators across newlines", "a<=b\n!c||d", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::LESS_EQUAL, "<=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::BANG, '!', 2, 1),
        At(TokenType::IDENTIFIER, "c", 2, 2),
        At(TokenType::OR_OR, "||", 2, 3),
        At(TokenType::IDENTIFIER, "d", 2, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 6)
    });

    return s.finish();
}
