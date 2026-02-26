#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <charconv>
#include <string>

namespace tkn
{

class Lexer
{
public:
  Lexer(std::string input) : input_{input} { read_char(); };
  void operator=(const Lexer &) = delete;
  void operator=(Lexer &&) = delete;

  void read_char()
  {
    if (read_pos_ >= input_.size())
    {
      ch_ = 0;
    }
    else
    {
      ch_ = input_[read_pos_];
    }

    pos_ = read_pos_;
    read_pos_++;
  };
  void skip_whitespace()
  {
    while (ch_ == ' ' || ch_ == '\t' || ch_ == '\n' || ch_ == '\r')
    {
      read_char();
    }
  };

  [[nodiscard]] char peek_char()
  {
    if (read_pos_ >= input_.size())
    {
      return 0;
    }
    else
    {
      return input_[read_pos_];
    }
  };

  [[nodiscard]] tkn::Token next_token()
  {
    tkn::Token tok;
    skip_whitespace();

    switch (ch_)
    {
    case '=':
      if (peek_char() == '=')
      {
        char ch = ch_;
        read_char();
        tok = tkn::Token{tkn::EQ, std::string(1, ch) + std::string(1, ch_)};
      }
      else
      {
        tok = new_token(tkn::ASSIGN, ch_);
      }
      break;

    case ';':
      tok = new_token(tkn::SEMICOLLON, ch_);
      break;
    case '(':
      tok = new_token(tkn::LPAREN, ch_);
      break;
    case ')':
      tok = new_token(tkn::RPAREN, ch_);
      break;
    case '+':
      tok = new_token(tkn::PLUS, ch_);
      break;
    case '{':
      tok = new_token(tkn::LBRACE, ch_);
      break;
    case '}':
      tok = new_token(tkn::RBRACE, ch_);
      break;
    case '-':
      tok = new_token(tkn::MINUS, ch_);
      break;
    case '/':
      tok = new_token(tkn::SLASH, ch_);
      break;

    case '*':
      tok = new_token(tkn::ASTERISK, ch_);
      break;

    case '<':
      tok = new_token(tkn::LT, ch_);
      break;
    case '>':
      tok = new_token(tkn::GT, ch_);
      break;

    case 0:
      tok.literal = "";
      tok.type = tkn::ENDOF;
      break;

    default:
      if (is_letter(ch_))
      {
        tok.literal = read_identifier();
        tok.type = tkn::LookupIdent(tok.literal);
        return tok;
      }
      else if (is_digit(ch_))
      {
        tok.type = tkn::INT;
        tok.literal = read_number();
        return tok;
      }
      else
      {
        tok = new_token(tkn::ILLEGAL, ch_);
      }
      break;
    }

    read_char();
    return tok;
  };

  [[nodiscard]] std::string read_identifier()
  {
    int pos = pos_;
    while (is_letter(ch_))
      read_char();
    return input_.substr(pos, pos_);
  };

  [[nodiscard]] std::string read_number()
  {
    int pos = pos_;
    while (is_digit(ch_))
      read_char();
    return input_.substr(pos, pos_);
  };

  [[nodiscard]] bool is_letter(char ch)
  {
    return 'a' <= ch && ch <= 'Z' || 'A' <= ch && ch <= 'Z' || ch == '_';
  };
  [[nodiscard]] bool is_digit(char ch) { return '0' <= ch && ch <= '9'; };
  [[nodiscard]] tkn::Token new_token(tkn::TokenType type, char ch)
  {
    return tkn::Token{type, std::string(1, ch)};
  };

private:
  std::string input_;
  int pos_;
  int read_pos_;
  char ch_;
};

} // namespace tkn

#endif // LEXER_
