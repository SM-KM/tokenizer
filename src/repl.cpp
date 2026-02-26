#include "repl.h"
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
  std::cout << ">> ";

  while (std::getline(std::cin, line))
  {
    if (line == "exit") break;
    std::cout << ">> ";
  }
};
