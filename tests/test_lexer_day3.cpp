#include "test_helpers.h"
using namespace test;
int main() {
    Suite s("Day 3");
    const auto eof = T(TokenType::ENDOFFILE,std::monostate{});
    s.check("integer declaration", "int x;", {T(TokenType::INT,"int"),T(TokenType::IDENTIFIER,"x"),T(TokenType::SEMI_COLON,';'),eof});
    s.check("float pointer declaration", "float* ptr;", {T(TokenType::FLOAT,"float"),T(TokenType::MULTIPLY,'*'),T(TokenType::IDENTIFIER,"ptr"),T(TokenType::SEMI_COLON,';'),eof});
    s.check("void function declaration", "void foo()", {T(TokenType::VOID,"void"),T(TokenType::IDENTIFIER,"foo"),T(TokenType::LEFT_PAREN,'('),T(TokenType::RIGHT_PAREN,')'),eof});
    s.check("shared memory array", "shared float data[256];", {T(TokenType::SHARED,"shared"),T(TokenType::FLOAT,"float"),T(TokenType::IDENTIFIER,"data"),T(TokenType::LEFT_BRACKET,'['),T(TokenType::INTEGER,256),T(TokenType::RIGHT_BRACKET,']'),T(TokenType::SEMI_COLON,';'),eof});
    s.check("global pointer", "global int* output;", {T(TokenType::GLOBAL,"global"),T(TokenType::INT,"int"),T(TokenType::MULTIPLY,'*'),T(TokenType::IDENTIFIER,"output"),T(TokenType::SEMI_COLON,';'),eof});
    s.check("kernel keyword at EOF", "kernel", {T(TokenType::KERNEL,"kernel"),eof});
    s.check("keyword prefix is identifier", "kernel123", {T(TokenType::IDENTIFIER,"kernel123"),eof});
    s.check("type prefix is identifier", "float32", {T(TokenType::IDENTIFIER,"float32"),eof});
    s.check("case sensitive keywords", "Kernel Float Shared Global", {T(TokenType::IDENTIFIER,"Kernel"),T(TokenType::IDENTIFIER,"Float"),T(TokenType::IDENTIFIER,"Shared"),T(TokenType::IDENTIFIER,"Global"),eof});
    s.check("keyword suffix remains identifier", "shared_data global_value integer", {T(TokenType::IDENTIFIER,"shared_data"),T(TokenType::IDENTIFIER,"global_value"),T(TokenType::IDENTIFIER,"integer"),eof});
    s.check("function call punctuation", "vector_add(a,b);", {T(TokenType::IDENTIFIER,"vector_add"),T(TokenType::LEFT_PAREN,'('),T(TokenType::IDENTIFIER,"a"),T(TokenType::COMMA,','),T(TokenType::IDENTIFIER,"b"),T(TokenType::RIGHT_PAREN,')'),T(TokenType::SEMI_COLON,';'),eof});
    s.check("parentheses", "()", {T(TokenType::LEFT_PAREN,'('),T(TokenType::RIGHT_PAREN,')'),eof});
    s.check("braces", "{}", {T(TokenType::LEFT_BRACE,'{'),T(TokenType::RIGHT_BRACE,'}'),eof});
    s.check("brackets", "[]", {T(TokenType::LEFT_BRACKET,'['),T(TokenType::RIGHT_BRACKET,']'),eof});
    s.check("comma semicolon", ",;", {T(TokenType::COMMA,','),T(TokenType::SEMI_COLON,';'),eof});
    s.check("punctuation at EOF", ")", {T(TokenType::RIGHT_PAREN,')'),eof});
    s.check("full GPU kernel declaration", "kernel vector_add(float* a, float* b, float* c, int n) {\n}", {
        T(TokenType::KERNEL,"kernel"),T(TokenType::IDENTIFIER,"vector_add"),T(TokenType::LEFT_PAREN,'('),
        T(TokenType::FLOAT,"float"),T(TokenType::MULTIPLY,'*'),T(TokenType::IDENTIFIER,"a"),T(TokenType::COMMA,','),
        T(TokenType::FLOAT,"float"),T(TokenType::MULTIPLY,'*'),T(TokenType::IDENTIFIER,"b"),T(TokenType::COMMA,','),
        T(TokenType::FLOAT,"float"),T(TokenType::MULTIPLY,'*'),T(TokenType::IDENTIFIER,"c"),T(TokenType::COMMA,','),
        T(TokenType::INT,"int"),T(TokenType::IDENTIFIER,"n"),T(TokenType::RIGHT_PAREN,')'),T(TokenType::LEFT_BRACE,'{'),T(TokenType::RIGHT_BRACE,'}'),eof
    });
    s.check("whitespace and punctuation", "\tint\n x ;\n", {T(TokenType::INT,"int"),T(TokenType::IDENTIFIER,"x"),T(TokenType::SEMI_COLON,';'),eof});
    s.check("invalid character recovery", "int x@;", {T(TokenType::INT,"int"),T(TokenType::IDENTIFIER,"x"),T(TokenType::ERROR),T(TokenType::SEMI_COLON,';'),eof});
    s.check("empty input", "", {eof});
    return s.finish();
}
