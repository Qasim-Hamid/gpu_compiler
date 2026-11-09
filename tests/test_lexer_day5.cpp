#include "test_helpers.h"

using namespace test;

int main() {
    Suite s("Day 5");

    // Single-line comments: skip all content until newline or EOF.
    s.check("single-line comment at EOF", "//comment", {
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 10)
    });
    s.check("empty single-line comment", "//", {
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 3)
    });
    s.check("single-line comment after identifier", "a//comment", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 11)
    });
    s.check("single-line comment followed by next line", "a//comment\nb", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::IDENTIFIER, "b", 2, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 2)
    });
    s.check("triple slash is single-line comment", "a///comment", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 12)
    });
    s.check("division is not a comment", "a/b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::DIVIDE, '/', 1, 2),
        At(TokenType::IDENTIFIER, "b", 1, 3),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 4)
    });

    // Block comments: skip through the first closing */ without emitting tokens.
    s.check("block comment only", "/*comment*/", {
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 12)
    });
    s.check("empty block comment", "/**/", {
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 5)
    });
    s.check("block comment between adjacent identifiers", "a/*comment*/b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::IDENTIFIER, "b", 1, 13),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 14)
    });
    s.check("empty block comment between identifiers", "a/**/b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::IDENTIFIER, "b", 1, 6),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 7)
    });
    s.check("multiline block comment preserves locations", "/*line1\nline2*/a", {
        At(TokenType::IDENTIFIER, "a", 2, 8),
        At(TokenType::ENDOFFILE, std::monostate{}, 2, 9)
    });
    s.check("block comment followed by operator", "a/*comment*/+b", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::PLUS, '+', 1, 13),
        At(TokenType::IDENTIFIER, "b", 1, 14),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 15)
    });
    s.check("first block-comment closer wins; no nesting", "/* outer /* inner */ x", {
        At(TokenType::IDENTIFIER, "x", 1, 22),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 23)
    });

    // Error tokens should point to the *opening* slash of an unterminated comment.
    s.check("unterminated block comment at start", "/* unfinished", {
        At(TokenType::ERROR, 1, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 14)
    });
    s.check("unterminated block comment after identifier", "a/* unfinished", {
        At(TokenType::IDENTIFIER, "a", 1, 1),
        At(TokenType::ERROR, 1, 2),
        At(TokenType::ENDOFFILE, std::monostate{}, 1, 15)
    });
    s.check("unterminated multiline comment", "kernel /*\n  unfinished\n", {
        At(TokenType::KERNEL, "kernel", 1, 1),
        At(TokenType::ERROR, 1, 8),
        At(TokenType::ENDOFFILE, std::monostate{}, 3, 1)
    });

    // Full Day 5 milestone, including accurate locations after skipped comments.
    s.check("GPU kernel with both comment styles",
        "// GPU vector addition\n"
        "kernel vector_add(float* a, float* b, float* c, int n) {\n"
        "    /* Each GPU thread\n"
        "       processes one element */\n"
        "    int i = 0;\n"
        "}", {
        At(TokenType::KERNEL, "kernel", 2, 1),
        At(TokenType::IDENTIFIER, "vector_add", 2, 8),
        At(TokenType::LEFT_PAREN, '(', 2, 18),
        At(TokenType::FLOAT, "float", 2, 19),
        At(TokenType::MULTIPLY, '*', 2, 24),
        At(TokenType::IDENTIFIER, "a", 2, 26),
        At(TokenType::COMMA, ',', 2, 27),
        At(TokenType::FLOAT, "float", 2, 29),
        At(TokenType::MULTIPLY, '*', 2, 34),
        At(TokenType::IDENTIFIER, "b", 2, 36),
        At(TokenType::COMMA, ',', 2, 37),
        At(TokenType::FLOAT, "float", 2, 39),
        At(TokenType::MULTIPLY, '*', 2, 44),
        At(TokenType::IDENTIFIER, "c", 2, 46),
        At(TokenType::COMMA, ',', 2, 47),
        At(TokenType::INT, "int", 2, 49),
        At(TokenType::IDENTIFIER, "n", 2, 53),
        At(TokenType::RIGHT_PAREN, ')', 2, 54),
        At(TokenType::LEFT_BRACE, '{', 2, 56),
        At(TokenType::INT, "int", 5, 5),
        At(TokenType::IDENTIFIER, "i", 5, 9),
        At(TokenType::EQUAL, '=', 5, 11),
        At(TokenType::INTEGER, 0, 5, 13),
        At(TokenType::SEMI_COLON, ';', 5, 14),
        At(TokenType::RIGHT_BRACE, '}', 6, 1),
        At(TokenType::ENDOFFILE, std::monostate{}, 6, 2)
    });

    return s.finish();
}
