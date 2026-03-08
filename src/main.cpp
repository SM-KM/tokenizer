#include "repl.h"
#include <iostream>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: program <file>\n";
    return 1;
  }

  run_file(argv[1]);
}
