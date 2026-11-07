#include "test_helpers.h"
using namespace test;
int main() {
    Suite s("Day 2");
    const auto eof = T(TokenType::ENDOFFILE,std::monostate{});
    s.check("integer at EOF", "123", {T(TokenType::INTEGER,123),eof});
    s.check("zero", "0", {T(TokenType::INTEGER,0),eof});
    s.check("multiple integers", "123 256 0", {T(TokenType::INTEGER,123),T(TokenType::INTEGER,256),T(TokenType::INTEGER,0),eof});
    s.check("floating point at EOF", "3.14", {T(TokenType::DOUBLE,3.14),eof});
    s.check("floating point variants", "0.5 10.0", {T(TokenType::DOUBLE,0.5),T(TokenType::DOUBLE,10.0),eof});
    s.check("addition without spaces", "1+2", {T(TokenType::INTEGER,1),T(TokenType::PLUS,'+'),T(TokenType::INTEGER,2),eof});
    s.check("negative number is two tokens", "-5", {T(TokenType::MINUS,'-'),T(TokenType::INTEGER,5),eof});
    s.check("multiplication expression", "a + b * 2", {T(TokenType::IDENTIFIER,"a"),T(TokenType::PLUS,'+'),T(TokenType::IDENTIFIER,"b"),T(TokenType::MULTIPLY,'*'),T(TokenType::INTEGER,2),eof});
    s.check("mixed expression without spaces", "12+3.5*2", {T(TokenType::INTEGER,12),T(TokenType::PLUS,'+'),T(TokenType::DOUBLE,3.5),T(TokenType::MULTIPLY,'*'),T(TokenType::INTEGER,2),eof});
    s.check("modulo", "1%2", {T(TokenType::INTEGER,1),T(TokenType::MODULO,'%'),T(TokenType::INTEGER,2),eof});
    s.check("division", "2/1.3", {T(TokenType::INTEGER,2),T(TokenType::DIVIDE,'/'),T(TokenType::DOUBLE,1.3),eof});
    // Current lexer consumes alphabetic suffixes of a numeric sequence as malformed.
    s.check("number followed by identifier characters", "12abc", {T(TokenType::ERROR),eof});
    s.check("invalid character recovery", "@123", {T(TokenType::ERROR),T(TokenType::INTEGER,123),eof});
    s.check("multiple decimal points", "3.14.5", {T(TokenType::ERROR),eof});
    s.check("trailing decimal point", "1.", {T(TokenType::ERROR),eof});
    s.check("empty input", "", {eof});
    return s.finish();
}
