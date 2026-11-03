#include "lexer.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

std::vector<std::string> tokenize(const std::string& source) {
    const auto tokens = process_input(source);
    std::vector<std::string> result;
    result.reserve(tokens.size());

    for (const auto& token : tokens) {
        std::ostringstream line;
        line << to_string(token.type) << ": " << token.value;
        result.push_back(line.str());
    }
    return result;
}

bool check(const std::string& name, const std::string& source,
           const std::vector<std::string>& expected) {
    const auto actual = tokenize(source);
    if (actual == expected) {
        std::cout << "PASS  " << name << '\n';
        return true;
    }

    std::cerr << "FAIL  " << name << "\n  input: " << source << "\n  expected:\n";
    for (const auto& line : expected) std::cerr << "    " << line << '\n';
    std::cerr << "  actual:\n";
    for (const auto& line : actual) std::cerr << "    " << line << '\n';
    return false;
}

} // namespace

int main() {
    int failures = 0;
    auto test = [&](const std::string& name, const std::string& source,
                    std::vector<std::string> expected) {
        if (!check(name, source, expected)) ++failures;
    };

    test("integer at EOF", "123", {"INT: 123", "EOF: N/A"});
    test("zero", "0", {"INT: 0", "EOF: N/A"});
    test("multiple integers", "123 256 0",
         {"INT: 123", "INT: 256", "INT: 0", "EOF: N/A"});
    test("floating point at EOF", "3.14", {"DBL: 3.14", "EOF: N/A"});
    test("floating point variants", "0.5 10.0",
         {"DBL: 0.5", "DBL: 10", "EOF: N/A"});
    test("addition without spaces", "1+2",
         {"INT: 1", "PLUS: +", "INT: 2", "EOF: N/A"});
    test("negative number is two tokens", "-5",
         {"MINUS: -", "INT: 5", "EOF: N/A"});
    test("multiplication expression", "a + b * 2",
         {"ID: a", "PLUS: +", "ID: b", "MULTIPLY: *", "INT: 2", "EOF: N/A"});
    test("mixed expression without spaces", "12+3.5*2",
         {"INT: 12", "PLUS: +", "DBL: 3.5", "MULTIPLY: *", "INT: 2", "EOF: N/A"});
    test("modulo", "1%2",
         {"INT: 1", "MOD: %", "INT: 2", "EOF: N/A"});
    test("division", "2/1.3",
         {"INT: 2", "DIV: /", "DBL: 1.3", "EOF: N/A"});
    test("number followed by identifier", "12abc",
         {"INT: 12", "ID: abc", "EOF: N/A"});
    test("invalid character recovery", "@123",
         {"ERR: @", "INT: 123", "EOF: N/A"});
    // These two tests use the whole-malformed-number behavior you demonstrated.
    test("multiple decimal points", "3.14.5",
         {"ERR: 3.14.5", "EOF: N/A"});
    test("trailing decimal point", "1.",
         {"ERR: 1.", "EOF: N/A"});
    test("empty input", "", {"EOF: N/A"});

    if (failures) {
        std::cerr << failures << " Day 2 test(s) failed.\n";
        return 1;
    }
    std::cout << "All Day 2 tests passed.\n";
    return 0;
}
