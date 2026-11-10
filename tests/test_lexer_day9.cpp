#include "test_helpers.h"

using namespace test;

// Day 9: compound assignment and increment/decrement operators.
// New tokens use std::string payloads; single-character operators use char.
// Adapt STAR_EQUAL / SLASH_EQUAL / MOD_EQUAL if your enum names differ.

int main() {
    Suite s("Day 9");

    s.check("addition assignment", "a+=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::PLUS_EQUAL, "+=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("subtraction assignment", "a-=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::MINUS_EQUAL, "-=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("multiplication assignment", "a*=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::STAR_EQUAL, "*=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("division assignment", "a/=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::SLASH_EQUAL, "/=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("modulo assignment", "a%=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::MOD_EQUAL, "%=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("prefix increment", "++i", {
        At(TokenType::PLUS_PLUS, "++", 1, 1),
        At(TokenType::IDENTIFIER, "i", 1, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    s.check("postfix increment", "i++", {
        At(TokenType::IDENTIFIER, "i", 1, 1),
        At(TokenType::PLUS_PLUS, "++", 1, 2),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    s.check("prefix decrement", "--i", {
        At(TokenType::MINUS_MINUS, "--", 1, 1),
        At(TokenType::IDENTIFIER, "i", 1, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    s.check("postfix decrement", "i--", {
        At(TokenType::IDENTIFIER, "i", 1, 1),
        At(TokenType::MINUS_MINUS, "--", 1, 2),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    s.check("triple plus takes longest match", "a+++b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::PLUS_PLUS, "++", 1, 2),
        At(TokenType::PLUS, '+', 1, 4),
        At(TokenType::IDENTIFIER, "b", 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("triple minus takes longest match", "a---b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::MINUS_MINUS, "--", 1, 2),
        At(TokenType::MINUS, '-', 1, 4),
        At(TokenType::IDENTIFIER, "b", 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("ordinary arithmetic remains single char", "a+b-c*d/e%f", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::PLUS, '+', 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 3),
        At(TokenType::MINUS, '-', 1, 4),
        At(TokenType::IDENTIFIER, "c", 1, 5),
        At(TokenType::MULTIPLY, '*', 1, 6),
        At(TokenType::IDENTIFIER, "d", 1, 7),
        At(TokenType::DIVIDE, '/', 1, 8),
        At(TokenType::IDENTIFIER, "e", 1, 9),
        At(TokenType::MODULO, '%', 1, 10),
        At(TokenType::IDENTIFIER, "f", 1, 11),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 12)
    });

    s.check("single division and block comment remain distinct", "a/b/*x*/c", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::DIVIDE, '/', 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 3),
        At(TokenType::IDENTIFIER, "c", 1, 9),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 10)
    });

    s.check("division assignment then line comment", "a/=b//ignore\nc", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::SLASH_EQUAL, "/=", 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 4),
        At(TokenType::IDENTIFIER, "c", 2, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 2)
    });

    s.check("line comment wins over division assignment", "a//=b\nc", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::IDENTIFIER, "c", 2, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 2)
    });

    s.check("integer before compound assignment", "1+=2", {
        At(TokenType::INTEGER, 1, 1, 1),
        At(TokenType::PLUS_EQUAL, "+=", 1, 2),
        At(TokenType::INTEGER, 2, 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });

    s.check("floating point after compound assignment", "sum+=3.14", {
        At(TokenType::IDENTIFIER, "sum", 1, 1),
        At(TokenType::PLUS_EQUAL, "+=", 1, 4),
        At(TokenType::DOUBLE, 3.14, 1, 6),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 10)
    });

    s.check("two operators across lines", "i++\n  j--", {
        At(TokenType::IDENTIFIER, "i", 1, 1),
        At(TokenType::PLUS_PLUS, "++", 1, 2),
        At(TokenType::IDENTIFIER, "j", 2, 3),
        At(TokenType::MINUS_MINUS, "--", 2, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 6)
    });

    s.check("block comment before compound assignment", "a/*x*/+=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::PLUS_EQUAL, "+=", 1, 7),
        At(TokenType::IDENTIFIER, "b", 1, 9),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 10)
    });

    s.check("adjacent prefix and postfix operators", "++i--", {
        At(TokenType::PLUS_PLUS, "++", 1, 1),
        At(TokenType::IDENTIFIER, "i", 1, 3),
        At(TokenType::MINUS_MINUS, "--", 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("star then star-equal longest match", "a**=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::MULTIPLY, '*', 1, 2),
        At(TokenType::STAR_EQUAL, "*=", 1, 3),
        At(TokenType::IDENTIFIER, "b", 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("percent equal followed by modulo", "n%=%2", {
        At(TokenType::IDENTIFIER, "n", 1, 1),
        At(TokenType::MOD_EQUAL, "%=", 1, 2),
        At(TokenType::MODULO, '%', 1, 4),
        At(TokenType::INTEGER, 2, 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });

    s.check("operator after comment and assignment", "a/*x*/=b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::EQUAL, '=', 1, 7),
        At(TokenType::IDENTIFIER, "b", 1, 8),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 9)
    });

    s.check("full for loop and accumulation", "for (int i = 0; i < n; i++) {\n    sum += a[i];\n}", {
        At(TokenType::FOR, "for", 1, 1),
        At(TokenType::LEFT_PAREN, '(', 1, 5),
        At(TokenType::INT, "int", 1, 6),
        At(TokenType::IDENTIFIER, "i", 1, 10),
        At(TokenType::EQUAL, '=', 1, 12),
        At(TokenType::INTEGER, 0, 1, 14),
        At(TokenType::SEMI_COLON, ';', 1, 15),
        At(TokenType::IDENTIFIER, "i", 1, 17),
        At(TokenType::LESS, '<', 1, 19),
        At(TokenType::IDENTIFIER, "n", 1, 21),
        At(TokenType::SEMI_COLON, ';', 1, 22),
        At(TokenType::IDENTIFIER, "i", 1, 24),
        At(TokenType::PLUS_PLUS, "++", 1, 25),
        At(TokenType::RIGHT_PAREN, ')', 1, 27),
        At(TokenType::LEFT_BRACE, '{', 1, 29),
        At(TokenType::IDENTIFIER, "sum", 2, 5),
        At(TokenType::PLUS_EQUAL, "+=", 2, 9),
        At(TokenType::IDENTIFIER, "a", 2, 12),
        At(TokenType::LEFT_BRACKET, '[', 2, 13),
        At(TokenType::IDENTIFIER, "i", 2, 14),
        At(TokenType::RIGHT_BRACKET, ']', 2, 15),
        At(TokenType::SEMI_COLON, ';', 2, 16),
        At(TokenType::RIGHT_BRACE, '}', 3, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 3, 2)
    });

    return s.finish();
}
