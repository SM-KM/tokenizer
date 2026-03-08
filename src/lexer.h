#ifndef LEXER_H
#define LEXER_H

#include <array>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#define LEXER_TRANSITIONS(X)                                                   \
  X(START, CHAR, IDENT)                                                        \
  X(START, NUM, INT)                                                           \
  X(START, EQ, ASSIGN_ST)                                                      \
  X(START, PLUS, PLUS_ST)                                                      \
  X(START, MINUS, MINUS_ST)                                                    \
  X(START, ASTERISK, ASTERISK_ST)                                              \
  X(START, SLASH, SLASH_ST)                                                    \
  X(START, LT, LT_ST)                                                          \
  X(START, GT, GT_ST)                                                          \
  X(START, LPAREN, LPAREN_ST)                                                  \
  X(START, RPAREN, RPAREN_ST)                                                  \
  X(START, LBRACE, LBRACE_ST)                                                  \
  X(START, RBRACE, RBRACE_ST)                                                  \
  X(IDENT, CHAR, IDENT)                                                        \
  X(INT, NUM, INT)                                                             \
  X(INT, DOT, FLOAT)                                                           \
  X(FLOAT, NUM, FLOAT)                                                         \
  X(ASSIGN_ST, EQ, EQ_ST)

namespace tkn
{

enum class TokenType
{
  IDENT,
  INT,
  FLOAT,
  ASSIGN,
  EQ,
  PLUS,
  MINUS,
  ASTERISK,
  SLASH,
  LT,
  GT,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  ENDOF,
  ILLEGAL,
  COUNT
};

static std::string to_string(tkn::TokenType type)
{
  switch (type)
  {
  case TokenType::IDENT:
    return "IDENT";
  case TokenType::INT:
    return "INT";
  case TokenType::FLOAT:
    return "FLOAT";
  case TokenType::ASSIGN:
    return "ASSIGN";
  case TokenType::EQ:
    return "EQ";
  case TokenType::PLUS:
    return "PLUS";
  case TokenType::MINUS:
    return "MINUS";
  case TokenType::ASTERISK:
    return "ASTERISK";
  case TokenType::SLASH:
    return "SLASH";
  case TokenType::LT:
    return "LT";
  case TokenType::GT:
    return "GT";
  case TokenType::LPAREN:
    return "LPAREN";
  case TokenType::RPAREN:
    return "RPAREN";
  case TokenType::LBRACE:
    return "LBRACE";
  case TokenType::RBRACE:
    return "RBRACE";
  case TokenType::ENDOF:
    return "ENDOF";
  case TokenType::ILLEGAL:
    return "ILLEGAL";
  case TokenType::COUNT:
    return "COUNT";
  default:
    std::abort();
  }
}

struct Token
{
  TokenType type;
  std::string literal;
};

class Lexer
{
public:
  explicit Lexer(std::string input) noexcept
      : input_{std::move(input)}, pos_{0} {};
  void operator=(const Lexer &) = delete;
  void operator=(Lexer &&) = delete;
  ~Lexer() = default;

  Token next_token()
  {
    skip_ws();
    if (pos_ >= input_.size()) return {TokenType::ENDOF, ""};

    State state = State::START;
    std::string buffer;

    while (true)
    {
      char ch = current_char();
      CharType char_type = classify(ch);
      State next =
          transitions_[static_cast<int>(state)][static_cast<int>(char_type)];
      if (next == State::DONE || state == State::DONE || next == State::ERROR)
        break;

      buffer += ch;
      state = next;
      advance();
    }
    return make_tokened(state, buffer);
  };

private:
  std::string input_;
  size_t pos_ = 0;

  enum class State
  {
    START,
    IDENT,
    INT,
    FLOAT,

    ASSIGN_ST,
    EQ_ST,

    PLUS_ST,
    MINUS_ST,
    ASTERISK_ST,
    SLASH_ST,
    LT_ST,
    GT_ST,
    LPAREN_ST,
    RPAREN_ST,
    LBRACE_ST,
    RBRACE_ST,

    DONE,
    ERROR,
    COUNT
  };

  enum class CharType
  {
    CHAR,
    NUM,
    DOT,
    EQ,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    LT,
    GT,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    WS,
    OTHER,
    END,
    COUNT
  };

  static constexpr size_t n_states = static_cast<size_t>(State::COUNT);
  static constexpr size_t n_chartypes = static_cast<size_t>(CharType::COUNT);
  using TransitionTable = std::array<std::array<State, n_chartypes>, n_states>;

  inline static constexpr TransitionTable make_table()
  {
    TransitionTable table{};
    for (auto &row : table)
      row.fill(State::DONE);
    table[(size_t)State::ERROR].fill(State::ERROR);

#define X(s, c, n) table[(size_t)State::s][(size_t)CharType::c] = State::n;
    LEXER_TRANSITIONS(X)
#undef X

    return table;
  }
  inline static TransitionTable transitions_ = make_table();

  char current_char() { return pos_ < input_.size() ? input_[pos_] : '\0'; };
  void advance() { ++pos_; };
  void skip_ws()
  {
    while (std::isspace(current_char()))
      advance();
  };

  CharType classify(char ch)
  {
    if (std::islower(ch)) return CharType::CHAR;
    if (std::isdigit(ch)) return CharType::NUM;
    if (ch == '.') return CharType::DOT;
    if (ch == '=') return CharType::EQ;
    if (ch == '+') return CharType::PLUS;
    if (ch == '-') return CharType::MINUS;
    if (ch == '*') return CharType::ASTERISK;
    if (ch == '/') return CharType::SLASH;
    if (ch == '<') return CharType::LT;
    if (ch == '>') return CharType::GT;
    if (ch == '(') return CharType::LPAREN;
    if (ch == ')') return CharType::RPAREN;
    if (ch == '{') return CharType::LBRACE;
    if (ch == '}') return CharType::RBRACE;
    if (std::isspace(ch)) return CharType::WS;
    if (ch == '\0') return CharType::END;
    return CharType::OTHER;
  };

  Token make_tokened(State state, const std::string &literal)
  {
    switch (state)
    {
    case State::IDENT:
      return {TokenType::IDENT, literal};
    case State::INT:
      return {TokenType::INT, literal};
    case State::FLOAT:
      return {TokenType::FLOAT, literal};
    case State::EQ_ST:
      if (literal == "=") return {TokenType::ASSIGN, literal};
      return {TokenType::EQ, literal};
    case State::DONE:
      return {TokenType::ILLEGAL, literal};
    case State::ASSIGN_ST:
      return {TokenType::ASSIGN, literal};
    case State::PLUS_ST:
      return {TokenType::PLUS, literal};
    case State::MINUS_ST:
      return {TokenType::MINUS, literal};
    case State::ASTERISK_ST:
      return {TokenType::ASTERISK, literal};
    case State::SLASH_ST:
      return {TokenType::SLASH, literal};
    case State::LT_ST:
      return {TokenType::LT, literal};
    case State::GT_ST:
      return {TokenType::GT, literal};
    case State::LPAREN_ST:
      return {TokenType::LPAREN, literal};
    case State::RPAREN_ST:
      return {TokenType::RPAREN, literal};
    case State::LBRACE_ST:
      return {TokenType::LBRACE, literal};
    case State::RBRACE_ST:
      return {TokenType::RBRACE, literal};
      break;
    }
  };
};

} // namespace tkn

#endif // LEXER_
