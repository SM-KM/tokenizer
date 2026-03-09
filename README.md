# Tokenizer

A C++20 lexer/tokenizer built with CMake and Clang.

## Overview

This project implements a tokenizer using a finite state machine (DFA).
It reads source files line by line and outputs tokens with their types and literals.

## Build System

- **Language**: C++20
- **Compiler**: Clang (clang++)
- **Build Tool**: CMake (3.16+)

## Building

```bash
cmake -B build -DCMAKE_CXX_COMPILER=clang++
cmake --build build
```

## Usage

```bash
./build/tkn <file>
```

The binary reads a source file and prints each token on its own line:
```
{Type: IDENT, Literal: x}
{Type: ASSIGN, Literal: =}
{Type: INT, Literal: 1}
```

## Supported Token Types

- `IDENT` - Identifiers (lowercase letters)
- `INT` - Integer literals
- `FLOAT` - Floating point literals
- `ASSIGN` - `=`
- `EQ` - `==`
- `PLUS`, `MINUS`, `ASTERISK`, `SLASH` - Arithmetic operators
- `LT`, `GT` - Comparison operators
- `LPAREN`, `RPAREN` - Parentheses
- `LBRACE`, `RBRACE` - Curly braces
- `ILLEGAL` - Unrecognized characters
- `ENDOF` - End of input
