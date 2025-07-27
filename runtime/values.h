#pragma once
#include <iostream>
#include <string>

// Defining the value types hell yeah
enum class ValueType {
  NULLVAL,
  NUMBER,
  BOOLEAN,
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

// BOOLEANS
struct BoolVal : RuntimeVal {
  bool value = true;
  BoolVal(bool v) : RuntimeVal(ValueType::BOOLEAN), value(v) {}

  std::string toString() const override {
    return std::to_string(value);
  }
};