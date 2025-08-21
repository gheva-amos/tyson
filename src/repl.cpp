#include <iostream>
#include <string>
#include <replxx.hxx>
#include <utility>
#include "parser/parser.h"
#include "lisp/env.h"

int main(int /*argc*/, char** /*argv*/)
{
  replxx::Replxx console;
  console.set_max_history_size(10000);
  console.set_no_color(false);

  std::unique_ptr<Env> environment = std::make_unique<Env>();

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

    try
    {
      Parser p{line};
      auto parsed{p.parse()};
      auto val(parsed->eval(environment));
      val = val.execute(environment);
      std::cout << val <<std::endl;
      console.history_add(line);
    }
    catch (const std::runtime_error& err)
    {
      std::cout << err.what() << std::endl;
      environment = std::make_unique<Env>();
    }
  }
  return 0;
}

