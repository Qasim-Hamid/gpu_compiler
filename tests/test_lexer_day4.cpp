#include "test_helpers.h"
using namespace test;
int main() {
    Suite s("Day 4");
    s.check("keyword and EOF", "kernel", {
        At(TokenType::KERNEL,"kernel",1,1), At(TokenType::ENDOFFILE,std::monostate{},1,7)});
    s.check("keyword followed by identifier", "kernel vector_add", {
        At(TokenType::KERNEL,"kernel",1,1), At(TokenType::IDENTIFIER,"vector_add",1,8),
        At(TokenType::ENDOFFILE,std::monostate{},1,18)});
    s.check("operators without spaces", "a+b", {
        At(TokenType::IDENTIFIER,"a",1,1),At(TokenType::PLUS,'+',1,2),
        At(TokenType::IDENTIFIER,"b",1,3),At(TokenType::ENDOFFILE,std::monostate{},1,4)});
    s.check("newline resets column", "a\nb", {
        At(TokenType::IDENTIFIER,"a",1,1),At(TokenType::IDENTIFIER,"b",2,1),
        At(TokenType::ENDOFFILE,std::monostate{},2,2)});
    s.check("indentation advances column", "a\n  b", {
        At(TokenType::IDENTIFIER,"a",1,1),At(TokenType::IDENTIFIER,"b",2,3),
        At(TokenType::ENDOFFILE,std::monostate{},2,4)});
    s.check("blank lines", "a\n\n b", {
        At(TokenType::IDENTIFIER,"a",1,1),At(TokenType::IDENTIFIER,"b",3,2),
        At(TokenType::ENDOFFILE,std::monostate{},3,3)});
    s.check("numeric literal position", "  123.45", {
        At(TokenType::DOUBLE,123.45,1,3),At(TokenType::ENDOFFILE,std::monostate{},1,9)});
    // An ERROR token's message text can evolve without breaking lexical tests.
    s.check("error at first character", "@", {
        At(TokenType::ERROR,1,1),At(TokenType::ENDOFFILE,std::monostate{},1,2)});
    s.check("error after newline", "a\n@", {
        At(TokenType::IDENTIFIER,"a",1,1),At(TokenType::ERROR,2,1),
        At(TokenType::ENDOFFILE,std::monostate{},2,2)});
    s.check("error after indentation", "a\n    @", {
        At(TokenType::IDENTIFIER,"a",1,1),At(TokenType::ERROR,2,5),
        At(TokenType::ENDOFFILE,std::monostate{},2,6)});
    s.check("empty input", "", {At(TokenType::ENDOFFILE,std::monostate{},1,1)});
    s.check("whitespace-only input", "  \n  ", {At(TokenType::ENDOFFILE,std::monostate{},2,3)});
    s.check("multiline GPU kernel and error",
        "kernel vector_add(float* a, float* b) {\n"
        "    int i = 0;\n"
        "    @\n"
        "}", {
        At(TokenType::KERNEL,"kernel",1,1),
        At(TokenType::IDENTIFIER,"vector_add",1,8),
        At(TokenType::LEFT_PAREN,'(',1,18),
        At(TokenType::FLOAT,"float",1,19),
        At(TokenType::MULTIPLY,'*',1,24),
        At(TokenType::IDENTIFIER,"a",1,26),
        At(TokenType::COMMA,',',1,27),
        At(TokenType::FLOAT,"float",1,29),
        At(TokenType::MULTIPLY,'*',1,34),
        At(TokenType::IDENTIFIER,"b",1,36),
        At(TokenType::RIGHT_PAREN,')',1,37),
        At(TokenType::LEFT_BRACE,'{',1,39),
        At(TokenType::INT,"int",2,5),
        At(TokenType::IDENTIFIER,"i",2,9),
        At(TokenType::EQUAL,'=',2,11),
        At(TokenType::INTEGER,0,2,13),
        At(TokenType::SEMI_COLON,';',2,14),
        At(TokenType::ERROR,3,5),
        At(TokenType::RIGHT_BRACE,'}',4,1),
        At(TokenType::ENDOFFILE,std::monostate{},4,2)
    });
    return s.finish();
}
