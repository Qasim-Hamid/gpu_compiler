# GPU Compiler — Lexer Specification (Day 10)

**Status:** Lexer v0.1 — rules established through Days 1–10.  
**Implementation language:** C++17.  
**Scope:** Tokenization only. A token stream is **not** a guarantee that a program is syntactically valid, well typed, or safe to execute on a GPU.

## 1. Lexical interface

The existing public entry point is:

```cpp
std::vector<Token> process_input(const std::string& source);
```

The lexer returns tokens in source order, including one final `ENDOFFILE` token. Each token has a type, a `TokenValue` payload, and a starting line/column. The program's command-line driver can read source files, display tokens, and print diagnostics; the lexer tests call `process_input()` directly.

`TokenValue` currently supports:

```cpp
std::variant<std::monostate, int, double, std::string, char, bool>
```

Payload conventions:

| Token category | Payload | Example |
|---|---|---|
| Identifier | `std::string` | `vector_add` |
| Reserved keyword (other than booleans) | `std::string` | `kernel` |
| Integer literal | `int` | `42` |
| Floating literal | `double` | `3.14` |
| Boolean literal | `bool` | `true` |
| Single-character punctuation/operator | `char` | `+`, `;` |
| Multi-character operator | `std::string` | `+=`, `==`, `&&` |
| EOF | `std::monostate` | no payload |
| Error | Diagnostic payload/message | see §7 |

The console's abbreviated display names (`KER`, `ID`, `DBL`, etc.) are **not** token type names. The tests use the `TokenType` enum rather than formatted output.

## 2. Whitespace and source positions

Spaces, tabs, and newlines separate tokens; they do not produce tokens. Line and column indices are **1-based**, and tokens retain their **starting** position. A consumed `\n` advances to the next line and resets the column to 1. EOF records the position immediately after the final source character.

Examples:

```text
""           -> EOF at 1:1
"abc"        -> IDENTIFIER at 1:1; EOF at 1:4
"a\nb"       -> a at 1:1; b at 2:1; EOF at 2:2
"/*x*/a"     -> a at 1:6
```

**Not yet specified:** carriage-return (`\r`) and Windows CRLF (`\r\n`) normalization. Do not assume CRLF behaves like a single newline until this is explicitly tested and documented.

## 3. Identifiers and reserved words

```ebnf
identifier = ("A"…"Z" | "a"…"z" | "_")
             { "A"…"Z" | "a"…"z" | "0"…"9" | "_" } ;
```

Identifiers are ASCII and case-sensitive. The **entire** identifier must match a reserved word; `if` is a keyword, but `iffy`, `if_condition`, and `IF` are identifiers.

| Category | Reserved words |
|---|---|
| Kernel | `kernel` |
| Types | `int`, `float`, `void` |
| GPU address-space qualifiers | `global`, `shared` |
| Control flow | `if`, `else`, `for`, `while` |
| Boolean literals | `true`, `false` |

`true` and `false` share a `BOOLEAN` token type and carry `bool` values. Other keywords carry their source spelling as strings. `global_id` is **not** a keyword: `global_id()` starts with an identifier followed by parentheses.

## 4. Numeric literals

```ebnf
integer_literal = digit, { digit } ;
floating_literal = digit, { digit }, ".", digit, { digit } ;
digit = "0"…"9" ;
```

| Input | Behavior |
|---|---|
| `123` | `INTEGER` with `int(123)` |
| `0.5`, `3.14` | `DOUBLE` with a `double` payload |
| `-2` | `MINUS` followed by `INTEGER(2)`; the sign is not part of the literal |
| `.5`, `1.` | Malformed numeric input: `ERROR` |
| `3.14.5`, `12abc`, `.5abc` | One `ERROR` token per malformed numeric sequence, per current lexer rules |
| Integer outside supported `int` range | `ERROR`, not an uncaught exception |

Exponent notation (`1e3`), hex, digit separators, and a dedicated float suffix are **not supported** in this version. The lexer does not perform arithmetic or operator-precedence evaluation.

## 5. Operators and punctuation

**Single-character operators:**

```text
+  -  *  /  %  =  <  >  !  &
```

**Multi-character operators:**

```text
==  !=  <=  >=  &&  ||
+=  -=  *=  /=  %=
++  --
```

**Punctuation:**

```text
(  )  {  }  [  ]  ,  ;
```

The lexer uses **longest-match** selection among recognized operators. Examples:

```text
"a==b"     -> ID, EQUAL_EQUAL, ID
"a+++b"    -> ID, PLUS_PLUS, PLUS, ID
"a---b"    -> ID, MINUS_MINUS, MINUS, ID
"a&&&b"    -> ID, AND_AND, AND, ID
```

This custom language recognizes a standalone `&` as `AND` (bitwise AND), but standalone `|` is **not** currently a valid token. `||` is recognized as logical OR. Neither operator's runtime semantics are defined at the lexer stage.

## 6. Comments

```cpp
// Runs until newline or end-of-file.

/* Runs until the first following closing delimiter. */
```

Comments produce **no tokens**, but all characters inside them count toward source positions. Block comments may span lines. **Nested block comments are unsupported**: the first `*/` ends the comment. A block comment without `*/` produces an `ERROR` at the opening `/*` position. A trailing `//` comment at EOF is valid.

A slash can begin `/`, `/=`, `//`, or `/*`, depending on lookahead. These cases must not be conflated.

## 7. Error handling

A lexical error is represented by an `ERROR` token, with a meaningful diagnostic and source location. Current tested categories include:

- Unexpected input character such as `@` or `#`.
- Malformed or out-of-range numeric literal.
- Unterminated `/* ...` block comment.

The scanner attempts recovery after an unexpected character, so multiple independent errors can appear in one token stream. An unterminated block comment consumes the remaining input and still results in an EOF token.

The CLI reports diagnostics including the source filename and positions. A successful lexical run returns exit status 0; a run with lexical errors returns nonzero (the current executable has been observed returning shell status 255, likely corresponding to `return -1`). Exact diagnostic wording and the specific nonzero exit code are not language-level requirements.

## 8. GPU-oriented source examples

A representative lexically valid kernel:

```cpp
kernel vector_add(global float* a, global float* b,
                  global float* c, int n) {
    int i = global_id();
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}
```

This is **lexically valid** because all words, literals, operators, and punctuation are recognized. The lexer does **not** enforce matching braces, declarations, type correctness, kernel calling conventions, bounds checks, or GPU safety.

In particular, the example `accumulate.gpu` is useful for testing `for`, `++`, and `+=`, but a future GPU implementation must address the race caused by multiple threads writing `output[0]`.

## 9. Interface contract for the parser

A future parser can depend on these properties:

1. Tokens appear in source order and include exactly one final `ENDOFFILE` token.
2. Comments and whitespace are omitted from the token stream.
3. `TokenType` disambiguates token roles; payload alternatives preserve literal types.
4. Each token carries a 1-based source start location for diagnostics.
5. Lexical errors are explicit tokens, rather than silent omissions.

**Deferred topics:** Unicode identifiers, CRLF normalization, hexadecimal/exponent numeric forms, richer literal types, additional operators, parser grammar, semantic type rules, GPU address-space semantics, and backend selection.

## 10. Regression verification

```bash
make test-day10
make clean && make test
```

The Day 10 suite covers complete vector addition, nested threshold conditionals, accumulation, interspersed comments, error recovery, and EOF/source positions. Days 1–9 supply 165 existing regression tests; Day 10 adds 15 more, for a target of **180** passing tests.

For additional memory- and undefined-behavior checks:

```bash
make clean
make test CXXFLAGS="-std=c++17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer"
```
