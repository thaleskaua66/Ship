#pragma once
#include <stdexcept>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include "values.h"

class Environment : public std::enable_shared_from_this<Environment> {
  private:
    std::shared_ptr<Environment> parent;
    std::map<std::string, std::shared_ptr<RuntimeVal>> variables;
    std::vector<std::string> constants;

  public:
  Environment() = default;

  explicit Environment(std::shared_ptr<Environment> parentEnv)
    : parent(std::move(parentEnv)) {}

  // Function to declare variables
  std::shared_ptr<RuntimeVal> declareVar(std::string name, std::shared_ptr<RuntimeVal> value, bool constant){
    if(variables.find(name) != variables.end()){
      throw std::runtime_error("Error: Variable already defined: " + name);
    }

    variables[name] = value;

    if(constant){
      constants.push_back(name);
    }

    return value;
  }

  // Function to assign variables
  std::shared_ptr<RuntimeVal> assignVar(std::string name, std::shared_ptr<RuntimeVal> value){
    std::shared_ptr<Environment> env = this->resolve(name);
    
    // check if it's constant
    auto element = std::find(constants.begin(), constants.end(), name);
    if(element != constants.end()){
      variables[name] = value;
    } else {
      throw std::runtime_error("Cannot resign to variable " + name + " since it's a constant.");
    }

    return value;
  }

  // Function to look to a var yeah that's what it means for sure
  std::shared_ptr<RuntimeVal> lookupVar(std::string name){
    std::shared_ptr<Environment> env = this->resolve(name);
    return env->variables.at(name);
  }

  // Resolving (i don't fully understand how it works, sorry :p)
  std::shared_ptr<Environment> resolve(std::string name){
    if(variables.find(name) != variables.end()){
      return shared_from_this();
    }

    if(!parent){
      throw std::runtime_error("Error: Cannot resolve: " + name);
    }

    return parent->resolve(name);
  }
};