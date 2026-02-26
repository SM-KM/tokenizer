#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>

namespace tkn
{
// consts to identify the tokens - keywords,
// identifiers, operators

const std::string ENDOF{"EOF"};
const std::string ILLEGAL{"ILLEGAL"};

// Identifies and literals
const std::string INT{"INT"};
const std::string FLOAT{"FLOAT"};
const std::string IDENT{"IDENT"};

// Operators
const std::string ASSIGN{"="};
const std::string PLUS{"+"};
const std::string MINUS{"-"};
const std::string BANG{"!"};
const std::string ASTERISK{"*"};
const std::string SLASH{"/"};
const std::string LT{"<"};
const std::string GT{">"};
const std::string EQ{"=="};
const std::string NOT_EQ{"!="};
const std::string SEMICOLLON{";"};
const std::string COMMA{","};
const std::string LPAREN{"("};
const std::string RPAREN{")"};
const std::string LBRACE{"{"};
const std::string RBRACE{"}"};

// Keywords
const std::string FUNCTION{"FUNCTION"};
const std::string LET{"LET"};
const std::string TRUE{"TRUE"};
const std::string FALSE{"FALSE"};
const std::string RETURN{"RETURN"};
const std::string IF{"IF"};
const std::string ELSE{"ELSE"};

using TokenType = std::string;
struct Token
{
  TokenType type;
  std::string literal;
};

static std::unordered_map<std::string, TokenType> keywords{
    {"fn", FUNCTION},   {"let", LET}, {"true", TRUE}, {"false", FALSE},
    {"return", RETURN}, {"if", IF},   {"else", ELSE}};

static TokenType LookupIdent(std::string ident)
{
  auto it = keywords.find(ident);
  if (it != keywords.end())
  {
    return it->second;
  }
  return IDENT;
}

} // namespace tkn
#endif // TOKEN_H
