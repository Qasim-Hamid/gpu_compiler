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

    test("keyword and identifier", "kernel vector_add",
         {"KER: kernel", "ID: vector_add", "EOF: N/A"});
    test("keyword at EOF", "kernel", {"KER: kernel", "EOF: N/A"});
    test("single-letter identifier at EOF", "a", {"ID: a", "EOF: N/A"});
    test("identifier at EOF", "vector_add", {"ID: vector_add", "EOF: N/A"});
    test("keyword prefix is identifier", "kernel123", {"ID: kernel123", "EOF: N/A"});
    test("case sensitive keyword", "Kernel", {"ID: Kernel", "EOF: N/A"});
    test("underscores", "_temp my_kernel",
         {"ID: _temp", "ID: my_kernel", "EOF: N/A"});
    test("spaces tabs newlines", "kernel \t\nvector_add",
         {"KER: kernel", "ID: vector_add", "EOF: N/A"});
    test("invalid character and recovery", "kernel@vector_add",
         {"KER: kernel", "ERR: @", "ID: vector_add", "EOF: N/A"});
    test("empty input", "", {"EOF: N/A"});
    test("whitespace only", " \t\n ", {"EOF: N/A"});

    if (failures) {
        std::cerr << failures << " Day 1 test(s) failed.\n";
        return 1;
    }
    std::cout << "All Day 1 tests passed.\n";
    return 0;
}
