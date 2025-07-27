#include "frontend/parser.h"
#include "runtime/interpreter.h"
#include "runtime/environment.h"
#include <iostream>

int main() {
  // Getting the parser and creating an environment for the program
  Parser parser;
  auto env = std::make_shared<Environment>();

  // some basic vars
  env->declareVar("null", std::make_shared<NullVal>(), true);
  env->declareVar("true", std::make_shared<BoolVal>(true), true);
  env->declareVar("false", std::make_shared<BoolVal>(false), true);

  // Printing a version cause it looks nice
  std::cout << "Ship v0.0.1" << std::endl;
  while(true){
    std::string input;

    std::cout << "> ";
    std::getline(std::cin, input);

    if(input == "exit"){
      break;
    }

    auto program = parser.produceAST(input);

    // Yes it's running the program that the user sent
    auto result = evaluate(program, env);
    std::cout << result->toString() << '\n' << std::endl;
  }

  return 0;
}