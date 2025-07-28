#pragma once
#include "values.h"
#include <charconv>
#include <memory>
#include <ostream>
#include <stdexcept>

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
  std::cout << std::endl;
  exit(1);
}

// HAIL = Asks something, basically an input.
inline std::shared_ptr<RuntimeVal> hail(
  const std::vector<std::shared_ptr<RuntimeVal>>& args,
  std::shared_ptr<Environment> env
){
  std::string value;
  std::cout << "-> Question, matey!" << std::endl;
  for(auto& arg : args) {
    std::cout << arg->toString() << " ";
  }
  std::getline(std::cin, value);

  return std::make_shared<StringVal>(value);
}

// TONUM = turns a value into a number, duh
inline std::shared_ptr<RuntimeVal> toNum(
  const std::vector<std::shared_ptr<RuntimeVal>>& args,
  std::shared_ptr<Environment> env
){
  if(args.empty()){
    std::cerr << "Wrong usage of toNum(). Must pass at least one param";
    exit(1);
  }

  auto val = args[0];

  switch(val->type){
    case ValueType::NUMBER: {
      return val;
    }
    case ValueType::STRING: {
      auto strVal = std::dynamic_pointer_cast<StringVal>(val);
      try{
        double number = std::stod(strVal->value);
        return std::make_shared<NumberVal>(number);
      }
      catch(const std::invalid_argument& e){
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        exit(1);
      }
      catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << e.what() << std::endl;
        exit(1);
      }
    }
    default: {
      std::cerr << "Error: toNum does only accepts strings!";
      exit(1);
    }
  }
}

// TOSTR = Turns a number into an string
inline std::shared_ptr<RuntimeVal> toStr(
  const std::vector<std::shared_ptr<RuntimeVal>>& args,
  std::shared_ptr<Environment> env
){
  if(args.empty()){
    std::cerr << "Wrong usage of toStr(). Must pass at least one param";
    exit(1);
  }

  auto val = args[0];

  switch(val->type){
    case ValueType::STRING: {
      return val;
    }
    case ValueType::NUMBER: {
      auto strVal = std::dynamic_pointer_cast<NumberVal>(val);
      try{
        std::string number = std::to_string(strVal->value);
        return std::make_shared<StringVal>(number);
      }
      catch(const std::invalid_argument& e){
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        exit(1);
      }
      catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << e.what() << std::endl;
        exit(1);
      }
    }
    default: {
      std::cerr << "Error: toNum does only accepts strings!";
      exit(1);
    }
  }
}

// WALKTHEPLANK = exit the program
inline std::shared_ptr<RuntimeVal> walkThePlank(
  const std::vector<std::shared_ptr<RuntimeVal>>& args,
  std::shared_ptr<Environment> env
){
  // Just leaves the program, yeah
  exit(1);
}