#include "frontend/parser.h"
#include "runtime/interpreter.h"
#include "runtime/environment.h"
#include <iostream>
#include <fstream>
#include <iterator>

int main(int argc, char* argv[]) {
  // Getting the parser and creating an environment for the program
  Parser parser;
  auto env = Environment::createGlobal();

  // Printing a version cause it looks nice
  std::cout << "Ship0.0.2" << std::endl;
  if(argc < 2){
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
  } else {
    std::ifstream file;

    file.open(argv[1]);
    if(!file.is_open()){
      std::cerr << "Error: Could not open file.";
      return 1;
    }

    // reading file and closing
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();

    auto program = parser.produceAST(content);
    std::cout << program << std::endl; // Will send the memory address if the program run succesfully
    // "Why don't you print the ast"? Cause i'm too lazy to create a function for that
    auto result = evaluate(program, env);

  }

  return 0;
}