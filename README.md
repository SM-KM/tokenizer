# Tokenizer

A C++20 lexer/tokenizer built with CMake and Clang.

# DFA
![DFA](/DFA.png)

## Explicación DFA

El siguiente **DFA (autómata finito determinista)** tiene cinco características particulares:

1. **Estado muerto**  
   Tenemos un estado muerto, el cual no tiene transiciones, ya que representa que se acaba el DFA; si lo vemos en el código, es el equivalente al fin del código.

2. **Estado INT**  
   El estado `INT` no tiene todas las transiciones; esto no es necesario, ya que definimos a todos los números positivos, incluyendo el `0`, como `N`.  
   Por lo tanto, no es necesario apuntarse a sí mismo cuando es un número positivo o `0`.

3. **Transiciones por color**  
   Cada color de cada flecha representa una transición diferente.

4. **Estado final**  
   El estado final no significa que se acabó el programa, simplemente indica que se imprimió un token válido.

5. **Reconocimiento de tokens completos**  
   Si tenemos la palabra `"hola"` no cuenta como identificador letra por letra, sino como toda la palabra.  
   Esto pasa igual con los números como `"124"`, por lo cual **no se imprime nada en la consola hasta que se cambie de estado**.

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
- `PLUS +`, `MINUS -`, `ASTERISK *`, `SLASH /` - Arithmetic operators
- `LT`, `GT` - Comparison operators
- `LPAREN`, `RPAREN` - Parentheses
- `LBRACE`, `RBRACE` - Curly braces
- `ILLEGAL` - Unrecognized characters
- `ENDOF` - End of input
