#include "frontend/parser.h"
#include "runtime/interpreter.h"
#include "runtime/environment.h"
#include <iostream>
#include <memory>

int main() {
  Parser parser;
  auto env = std::make_shared<Environment>();

  // vars
  env->declareVar("null", std::make_shared<NullVal>(), true);
  env->declareVar("true", std::make_shared<BoolVal>(true), true);
  env->declareVar("false", std::make_shared<BoolVal>(false), true);

  std::cout << "Ship @0.1" << std::endl;
  while(true){
    std::string input;
    std::cout << "> ";
    std::getline(std::cin, input);

    if(input == "exit"){
      break;
    }

    auto program = parser.produceAST(input);
    std::cout << '\n';

    auto result = evaluate(program, env);
    std::cout << result->toString() << std::endl;
  }

  return 0;
}