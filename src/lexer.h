#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <cctype>
#include <charconv>
#include <iostream>
#include <string>
#include <utility>

namespace tkn
{

class Lexer
{
public:
  Lexer(std::string input)
      : input_{std::move(input)}, ch_{0}, pos_{0}, read_pos_{0}
  {
    read_char();
  };

  ~Lexer() = default;
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
    tkn::Token tok{};
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
        if (is_digit(peek_char()))
        {
          read_number(tok);
          return tok;
        }

        tok.literal = read_identifier();
        tok.type = tkn::LookupIdent(tok.literal);
        return tok;
      }
      else if (is_digit(ch_))
      {
        read_number(tok);
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
    return input_.substr(pos, pos_ - pos);
  };

  void read_number(tkn::Token &tok)
  {
    int start_pos = pos_;
    bool has_dot = false;
    while (true)
    {
      if (ch_ == '.')
      {
        if (has_dot) break;
        has_dot = true;
      }
      else if (!is_digit(ch_))
      {
        break;
      }
      read_char();
    }
    tok.type = has_dot ? tkn::FLOAT : tkn::INT;
    tok.literal = input_.substr(start_pos, pos_ - start_pos);
  };

  [[nodiscard]] bool is_letter(char ch)
  {
    return std::isalpha(static_cast<unsigned char>(ch)) || ch == '_' ||
           ch == '.';
  };
  [[nodiscard]] bool is_digit(char ch) { return '0' <= ch && ch <= '9'; };
  [[nodiscard]] bool is_float(char ch)
  {
    return ('0' <= ch && ch <= '9') || ch == '.';
  };
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
