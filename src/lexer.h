#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>

namespace tkn
{

class Lexer
{
public:
  Lexer(std::string input) : input_{input} { read_char(); };
  void operator=(const Lexer &) = delete;
  void operator=(Lexer &&) = delete;

  void read_char();
  void skip_whitespace();

  [[nodiscard]] char peek_char();
  [[nodiscard]] tkn::Token next_token();
  [[nodiscard]] std::string read_identifier();

  [[nodiscard]] std::string read_number();
  [[nodiscard]] std::string is_letter();
  [[nodiscard]] std::string is_digit();
  [[nodiscard]] tkn::Token new_token();

private:
  std::string input_;
  int pos_;
  int read_pos_;
  char ch;
};

} // namespace tkn

#endif // LEXER_
