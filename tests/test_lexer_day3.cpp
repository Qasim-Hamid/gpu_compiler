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

    // Data-type keywords and punctuation
    test("integer declaration", "int x;",
         {"INT_TYPE: int", "ID: x", "SCOLON: ;", "EOF: N/A"});
    test("float pointer declaration", "float* ptr;",
         {"FLOAT_TYPE: float", "MULTIPLY: *", "ID: ptr", "SCOLON: ;", "EOF: N/A"});
    test("void function declaration", "void foo()",
         {"VOID: void", "ID: foo", "LPAREN: (", "RPAREN: )", "EOF: N/A"});
    test("shared memory array", "shared float data[256];",
         {"SHARED: shared", "FLOAT_TYPE: float", "ID: data", "LBRACKET: [",
          "INT: 256", "RBRACKET: ]", "SCOLON: ;", "EOF: N/A"});
    test("global pointer", "global int* output;",
         {"GLOBAL: global", "INT_TYPE: int", "MULTIPLY: *", "ID: output",
          "SCOLON: ;", "EOF: N/A"});

    // Keyword boundaries and regressions
    test("kernel keyword at EOF", "kernel", {"KER: kernel", "EOF: N/A"});
    test("keyword prefix is identifier", "kernel123", {"ID: kernel123", "EOF: N/A"});
    test("type prefix is identifier", "float32", {"ID: float32", "EOF: N/A"});
    test("case sensitive keywords", "Kernel Float Shared Global",
         {"ID: Kernel", "ID: Float", "ID: Shared", "ID: Global", "EOF: N/A"});
    test("keyword suffix remains identifier", "shared_data global_value integer",
         {"ID: shared_data", "ID: global_value", "ID: integer", "EOF: N/A"});

    // Punctuation, including adjacent tokens without whitespace
    test("function call punctuation", "vector_add(a,b);",
         {"ID: vector_add", "LPAREN: (", "ID: a", "COMMA: ,", "ID: b",
          "RPAREN: )", "SCOLON: ;", "EOF: N/A"});
    test("parentheses", "()", {"LPAREN: (", "RPAREN: )", "EOF: N/A"});
    test("braces", "{}", {"LBRACE: {", "RBRACE: }", "EOF: N/A"});
    test("brackets", "[]", {"LBRACKET: [", "RBRACKET: ]", "EOF: N/A"});
    test("comma semicolon", ",;", {"COMMA: ,", "SCOLON: ;", "EOF: N/A"});
    test("punctuation at EOF", ")", {"RPAREN: )", "EOF: N/A"});

    // Main Day 3 milestone: a complete, multi-line kernel declaration.
    test("full GPU kernel declaration",
         "kernel vector_add(float* a, float* b, float* c, int n) {\n}",
         {"KER: kernel", "ID: vector_add", "LPAREN: (",
          "FLOAT_TYPE: float", "MULTIPLY: *", "ID: a", "COMMA: ,",
          "FLOAT_TYPE: float", "MULTIPLY: *", "ID: b", "COMMA: ,",
          "FLOAT_TYPE: float", "MULTIPLY: *", "ID: c", "COMMA: ,",
          "INT_TYPE: int", "ID: n", "RPAREN: )", "LBRACE: {",
          "RBRACE: }", "EOF: N/A"});
    test("whitespace and punctuation", "\tint\n x ;\n",
         {"INT_TYPE: int", "ID: x", "SCOLON: ;", "EOF: N/A"});
    test("invalid character recovery", "int x@;",
         {"INT_TYPE: int", "ID: x", "ERR: @", "SCOLON: ;", "EOF: N/A"});
    test("empty input", "", {"EOF: N/A"});

    if (failures) {
        std::cerr << failures << " Day 3 test(s) failed.\n";
        return 1;
    }
    std::cout << "All Day 3 tests passed.\n";
    return 0;
}
