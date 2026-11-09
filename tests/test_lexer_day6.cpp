#include "test_helpers.h"

using namespace test;

int main() {
    Suite s("Day 6");

    // Error recovery: an unexpected character must not stop later tokens.
    // At(TokenType::ERROR, line, column) deliberately ignores the wording
    // of the error message and verifies the error's type and location.
    s.check("two consecutive invalid characters", "@#", {
        At(TokenType::ERROR, 1, 1),
        At(TokenType::ERROR, 1, 2),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 3)
    });
    s.check("recover between identifiers", "a@b#c", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::ERROR, 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 3),
        At(TokenType::ERROR, 1, 4),
        At(TokenType::IDENTIFIER, "c", 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });
    s.check("recover around operators and punctuation", "int a = @ + #;", {
        At(TokenType::INT, "int", 1, 1),
        At(TokenType::IDENTIFIER, "a", 1, 5),
        At(TokenType::EQUAL, '=', 1, 7),
        At(TokenType::ERROR, 1, 9),
        At(TokenType::PLUS, '+', 1, 11),
        At(TokenType::ERROR, 1, 13),
        At(TokenType::SEMI_COLON, ';', 1, 14),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 15)
    });
    s.check("recover after error on subsequent line", "a@\nb", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::ERROR, 1, 2),
        At(TokenType::IDENTIFIER, "b", 2, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 2)
    });

    // Valid token boundaries, including no whitespace between tokens.
    s.check("arithmetic precedence does not affect lexing", "1+2*3", {
        At(TokenType::INTEGER, 1, 1, 1),
        At(TokenType::PLUS, '+', 1, 2),
        At(TokenType::INTEGER, 2, 1, 3),
        At(TokenType::MULTIPLY, '*', 1, 4),
        At(TokenType::INTEGER, 3, 1, 5),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });
    s.check("number directly before semicolon", "123;", {
        At(TokenType::INTEGER, 123, 1, 1),
        At(TokenType::SEMI_COLON, ';', 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });
    s.check("number inside array brackets", "data[256]", {
        At(TokenType::IDENTIFIER, "data", 1, 1),
        At(TokenType::LEFT_BRACKET, '[', 1, 5),
        At(TokenType::INTEGER, 256, 1, 6),
        At(TokenType::RIGHT_BRACKET, ']', 1, 9),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 10)
    });
    s.check("floating point before multiplication", "3.14*2", {
        At(TokenType::DOUBLE, 3.14, 1, 1),
        At(TokenType::MULTIPLY, '*', 1, 5),
        At(TokenType::INTEGER, 2, 1, 6),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 7)
    });

    // Language rules established on Day 2:
    // .5 and 1. are invalid; a malformed numeric sequence is ONE error.
    s.check("leading decimal point", ".5", {
        At(TokenType::ERROR, 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 3)
    });
    s.check("leading decimal with suffix is one error", ".5abc", {
        At(TokenType::ERROR, 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });
    s.check("recover after malformed decimal", ".5+2", {
        At(TokenType::ERROR, 1, 1),
        At(TokenType::PLUS, '+', 1, 3),
        At(TokenType::INTEGER, 2, 1, 4),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });
    s.check("trailing decimal point", "1.", {
        At(TokenType::ERROR, 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 3)
    });
    s.check("multiple decimal points are one error", "3.14.5", {
        At(TokenType::ERROR, 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 7)
    });
    s.check("numeric identifier suffix is one error", "12abc", {
        At(TokenType::ERROR, 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 6)
    });
    s.check("integer overflow gives error without throwing", "2147483648", {
        At(TokenType::ERROR, 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 11)
    });

    // Day 5 regression tests: precise positions after comments.
    s.check("newline after line comment is counted once", "a//comment\nb", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::IDENTIFIER, "b", 2, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 2)
    });
    s.check("closing block comment consumes both characters", "a/*comment*/b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::IDENTIFIER, "b", 1, 13),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 14)
    });
    s.check("unterminated block comment stops at EOF", "kernel /*unfinished", {
        At(TokenType::KERNEL, "kernel", 1, 1),
        At(TokenType::ERROR, 1, 8),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 20)
    });

    return s.finish();
}
