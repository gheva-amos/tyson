#include <iostream>
#include <string>
#include <replxx.hxx>
#include <utility>

int main(int /*argc*/, char** /*argv*/)
{
  replxx::Replxx console;
  console.set_max_history_size(10000);
  console.set_no_color(false);
  while (true)
  {
    const char* cline = console.input(" >>> ");
    if (!cline)
    {
      std::cout << std::endl << "buh bye" << std::endl;
      break;
    }
    std::string line{std::move(cline)};
    if (line.empty())
    {
      continue;
    }
    console.history_add(line);

    std::cout << line <<std::endl;
  }
  return 0;
}

