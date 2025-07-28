#pragma once
#include "values.h"
#include <memory>
#include <ostream>

// THIS FILE INCLUDES ALL NATIVE FUNCTIONS (AKA MAPS IN SHIP)

// YELL = Works as a print()
inline std::shared_ptr<RuntimeVal> yell(
  const std::vector<std::shared_ptr<RuntimeVal>>& args,
  std::shared_ptr<Environment> env
){
  for(auto& arg : args){
    std::cout << arg->toString() << " ";
  }
  std::cout << '\n';
  return std::shared_ptr<NullVal>();
}

// SCREAM = works as an error sender, will close the program with a message
inline std::shared_ptr<RuntimeVal> scream(
  const std::vector<std::shared_ptr<RuntimeVal>>& args,
  std::shared_ptr<Environment> env
){
  std::cout << std::endl;
  std::cout << "GRRRR!" << std::endl;
  for(auto& arg : args) {
    std::cout << arg->toString() << " ";
  }
  std::cout << std::endl; // no performance problem clearing buffer since program will end, duh
  exit(1);
}