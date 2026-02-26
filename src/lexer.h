#ifndef LEXER_H
#define LEXER_H

#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

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
    return "UNKNOWN";
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

  static constexpr int n_states = static_cast<size_t>(State::COUNT);
  static constexpr int n_chartypes = static_cast<size_t>(CharType::COUNT);

  State transitions_[n_states][n_chartypes] = {
      {State::IDENT, State::INT, State::ERROR, State::ASSIGN_ST, State::PLUS_ST,
       State::MINUS_ST, State::ASTERISK_ST, State::SLASH_ST, State::LT_ST,
       State::GT_ST, State::LPAREN_ST, State::RPAREN_ST, State::LBRACE_ST,
       State::RBRACE_ST, State::START, State::ERROR, State::DONE},

      {State::IDENT, State::IDENT, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::INT, State::FLOAT, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::FLOAT, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::EQ_ST, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE, State::DONE, State::DONE, State::DONE,
       State::DONE, State::DONE},

      {State::ERROR, State::ERROR, State::ERROR, State::ERROR, State::ERROR,
       State::ERROR, State::ERROR, State::ERROR, State::ERROR, State::ERROR,
       State::ERROR, State::ERROR, State::ERROR, State::ERROR, State::ERROR,
       State::ERROR, State::ERROR}};

  char current_char() { return pos_ < input_.size() ? input_[pos_] : '\0'; };
  void advance() { ++pos_; };
  void skip_ws()
  {
    while (std::isspace(current_char()))
      advance();
  };

  CharType classify(char ch)
  {
    if (std::isalpha(ch)) return CharType::CHAR;
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
