#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "../frontend/ast.h" // to get statements


// Because can't include environment.h
class Environment;

// Defining the value types hell yeah
enum class ValueType {
  NULLVAL,
  NUMBER,
  BOOLEAN,
  STRING,
  OBJECT,
  NATIVEFUNCTION,
  FUNCTION,
};

// Polimorphism to all the values (NULLVAL, NUMBER, etc)
struct RuntimeVal {
  ValueType type;
  RuntimeVal(ValueType t) : type(t) {}
  virtual ~RuntimeVal() {}

  virtual std::string toString() const = 0;
};

// NULL
struct NullVal : RuntimeVal {
  NullVal() : RuntimeVal(ValueType::NULLVAL) {}

  std::string toString() const override {
    return "null";
  }
};


// NUMBERS
struct NumberVal : RuntimeVal {
  double value;
  NumberVal(double v) : RuntimeVal(ValueType::NUMBER), value(v) {}

  std::string toString() const override {
    return std::to_string(value);
  }
};

// STRINGS
struct StringVal : RuntimeVal {
  std::string value;

  StringVal(std::string v)
  : RuntimeVal(ValueType::STRING), value(v) {}

  std::string toString() const override {
    return value;
  }
};

// BOOLEANS
struct BoolVal : RuntimeVal {
  bool value = true;
  BoolVal(bool v) : RuntimeVal(ValueType::BOOLEAN), value(v) {}

  std::string toString() const override {
    return std::to_string(value);
  }
};

// OBJECTS
struct ObjectVal : RuntimeVal {
  std::unordered_map<std::string, std::shared_ptr<RuntimeVal>> properties;
  
  ObjectVal() : RuntimeVal(ValueType::OBJECT) {}

  std::string toString() const override {
    return "[object]";
  }
};

using FunctionCall = std::function<std::shared_ptr<RuntimeVal>(
  const std::vector<std::shared_ptr<RuntimeVal>>& args,
  std::shared_ptr<Environment> env
)>;

struct NativeFunctionVal : RuntimeVal {
  FunctionCall call;

  NativeFunctionVal(FunctionCall fn)
    : RuntimeVal(ValueType::NATIVEFUNCTION), call(fn) {}
    
  std::string toString() const override {
    return "native-fn";
  }
};

struct FunctionVal : RuntimeVal {
  std::string name;
  std::vector<std::string> parameters;
  std::shared_ptr<Environment> declarationEnv;
  std::vector<std::shared_ptr<Statement>> body;

  FunctionVal(std::string nm, std::vector<std::string> params, std::shared_ptr<Environment> scope, std::vector<std::shared_ptr<Statement>> bd)
    : RuntimeVal(ValueType::FUNCTION), name(nm), parameters(params), declarationEnv(scope), body(bd) {}
    
  std::string toString() const override {
    return "function";
  }
};