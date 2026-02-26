#include "repl.h"
#include "lexer.h"

#include <iostream>
#include <string>

void start_repl()
{
  std::string line;
  while (true)
  {
    std::getline(std::cin, line);
    if (line == "exit") break;
    tkn::Lexer l{line};
    while (true)
    {
      auto tok = l.next_token();
      if (tok.type == tkn::TokenType::ENDOF) break;
      std::cout << "{Type: " << tkn::to_string(tok.type)
                << ", Literal: " << tok.literal << "}\n";
    }
  };
}
