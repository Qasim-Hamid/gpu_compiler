#pragma once

#include "lexer.h"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace test {

// All tests inspect Token directly. No output parsing or display-label comparisons.
struct Expected {
    TokenType type;
    std::optional<TokenValue> value; // Empty: do not check payload (useful for diagnostics).
    std::size_t line = 0;            // Zero: position not checked.
    std::size_t column = 0;
};

inline Expected T(TokenType type, TokenValue value) { return {type, std::move(value)}; }
inline Expected T(TokenType type) { return {type, std::nullopt}; }
inline Expected At(TokenType type, TokenValue value, std::size_t line, std::size_t column) {
    return {type, std::move(value), line, column};
}
inline Expected At(TokenType type, std::size_t line, std::size_t column) {
    return {type, std::nullopt, line, column};
}

// Match either token.line/column or token.location.line/column.
// Update these two accessors if your Token uses different field names.
template<typename T, typename = void>
struct has_direct_location : std::false_type {};
template<typename T>
struct has_direct_location<T, std::void_t<decltype(std::declval<const T&>().line),
                                          decltype(std::declval<const T&>().column)>> : std::true_type {};

template<typename T, typename = void>
struct has_nested_location : std::false_type {};
template<typename T>
struct has_nested_location<T, std::void_t<decltype(std::declval<const T&>().location.line),
                                          decltype(std::declval<const T&>().location.column)>> : std::true_type {};

template<typename T>
std::size_t token_line(const T& token) {
    if constexpr (has_direct_location<T>::value) return token.line;
    else if constexpr (has_nested_location<T>::value) return token.location.line;
    else { static_assert(has_direct_location<T>::value || has_nested_location<T>::value,
                         "Adjust token_line(): Token needs line/column or location.line/column"); return 0; }
}

template<typename T>
std::size_t token_column(const T& token) {
    if constexpr (has_direct_location<T>::value) return token.column;
    else if constexpr (has_nested_location<T>::value) return token.location.column;
    else { static_assert(has_direct_location<T>::value || has_nested_location<T>::value,
                         "Adjust token_column(): Token needs line/column or location.line/column"); return 0; }
}

inline bool same_value(const TokenValue& a, const TokenValue& b) {
    if (a.index() != b.index()) return false;
    if (std::holds_alternative<double>(a)) {
        return std::fabs(std::get<double>(a) - std::get<double>(b)) < 1e-10;
    }
    return a == b;
}

inline void show_value(const TokenValue& value) {
    std::visit([](const auto& x) {
        using V = std::decay_t<decltype(x)>;
        if constexpr (std::is_same_v<V, std::monostate>) std::cerr << "<empty>";
        else std::cerr << x;
    }, value);
}

class Suite {
public:
    explicit Suite(const char* name) : name_(name) {}

    void check(const std::string& label, const std::string& source,
               const std::vector<Expected>& expected) {
        const std::vector<Token> actual = process_input(source);
        bool ok = actual.size() == expected.size();
        if (ok) {
            for (std::size_t i = 0; i < actual.size(); ++i) {
                const auto& a = actual[i];
                const auto& e = expected[i];
                if (a.type != e.type ||
                    (e.value && !same_value(a.value, *e.value)) ||
                    (e.line && token_line(a) != e.line) ||
                    (e.column && token_column(a) != e.column)) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            std::cout << "PASS  " << label << '\n';
            return;
        }
        ++failures_;
        std::cerr << "FAIL  " << label << " (input: " << source << ")\n";
        std::cerr << "  expected " << expected.size() << " token(s), got " << actual.size() << '\n';
        const std::size_t count = expected.size() < actual.size() ? expected.size() : actual.size();
        for (std::size_t i = 0; i < count; ++i) {
            const auto& a = actual[i];
            const auto& e = expected[i];
            const bool mismatch = a.type != e.type ||
                (e.value && !same_value(a.value, *e.value)) ||
                (e.line && token_line(a) != e.line) ||
                (e.column && token_column(a) != e.column);
            if (mismatch) {
                std::cerr << "  token #" << i << ": type expected=" << static_cast<int>(e.type)
                          << " actual=" << static_cast<int>(a.type) << '\n';
                if (e.value) { std::cerr << "    value expected="; show_value(*e.value);
                               std::cerr << " actual="; show_value(a.value); std::cerr << '\n'; }
                if (e.line || e.column) {
                    std::cerr << "    position expected=" << e.line << ':' << e.column
                              << " actual=" << token_line(a) << ':' << token_column(a) << '\n';
                }
            }
        }
    }

    int finish() const {
        if (failures_) {
            std::cerr << failures_ << ' ' << name_ << " test(s) failed.\n";
            return 1;
        }
        std::cout << "All " << name_ << " tests passed.\n";
        return 0;
    }

private:
    const char* name_;
    int failures_ = 0;
};

} // namespace test
