#include "test_helpers.h"
using namespace test;
int main() {
    Suite s("Day 1");
    const auto eof = T(TokenType::ENDOFFILE, std::monostate{});
    s.check("keyword and identifier", "kernel vector_add", {T(TokenType::KERNEL,"kernel"),T(TokenType::IDENTIFIER,"vector_add"),eof});
    s.check("keyword at EOF", "kernel", {T(TokenType::KERNEL,"kernel"),eof});
    s.check("single-letter identifier at EOF", "a", {T(TokenType::IDENTIFIER,"a"),eof});
    s.check("identifier at EOF", "vector_add", {T(TokenType::IDENTIFIER,"vector_add"),eof});
    s.check("keyword prefix is identifier", "kernel123", {T(TokenType::IDENTIFIER,"kernel123"),eof});
    s.check("case sensitive keyword", "Kernel", {T(TokenType::IDENTIFIER,"Kernel"),eof});
    s.check("underscores", "_temp my_kernel", {T(TokenType::IDENTIFIER,"_temp"),T(TokenType::IDENTIFIER,"my_kernel"),eof});
    s.check("spaces tabs newlines", "kernel \t\nvector_add", {T(TokenType::KERNEL,"kernel"),T(TokenType::IDENTIFIER,"vector_add"),eof});
    // ERROR payload wording is tested separately from lexing behavior.
    s.check("invalid character and recovery", "kernel@vector_add", {T(TokenType::KERNEL,"kernel"),T(TokenType::ERROR),T(TokenType::IDENTIFIER,"vector_add"),eof});
    s.check("empty input", "", {eof});
    s.check("whitespace only", " \t\n ", {eof});
    return s.finish();
}
