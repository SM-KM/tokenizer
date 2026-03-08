#include "repl.h"
#include "lexer.h"

#include <fstream>
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

void run_file(const std::string &path)
{
  std::ifstream file(path);
  if (!file)
  {
    std::cerr << "Failed to open file: " << path << "\n";
    std::abort();
  }

  std::string line;

  while (std::getline(file, line))
  {
    tkn::Lexer l{line};

    while (true)
    {
      auto tok = l.next_token();
      if (tok.type == tkn::TokenType::ENDOF) break;

      std::cout << "{Type: " << tkn::to_string(tok.type)
                << ", Literal: " << tok.literal << "}\n";
    }
  }
}
