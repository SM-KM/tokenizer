#include "repl.h"
#include "lexer.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

void start_repl()
{
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::string line;
  std::cout << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X")
            << " Feel free to type a command\n";
  while (std::getline(std::cin, line))
  {
    if (line == "exit") break;
    tkn::Lexer l{line};
    for (auto tok = l.next_token(); tok.type != tkn::ENDOF;)
    {
      std::cout << "{Type: " << tok.type << ", Literal: " << tok.literal
                << "}\n";
      tok = l.next_token();
    }
  }
};
