#pragma once
#include <memory>
#include <string>
#include <vector>
#include <optional>

enum class NodeType {
  // STATEMENTS
  Program,
  VarDeclaration,

  // EXPRESSIONS
  NumericLiteral,
  Identifier,
  BinaryExpr,
  CallExpr,
  UnaryExpr,
  FunctionDeclaration,
};

struct Statement {
  virtual NodeType kind() const = 0;
  virtual ~Statement() {};
};

// Getting the body of the program, so we can execute it later on
struct Program : public Statement {
  std::vector<std::shared_ptr<Statement>> body;

  NodeType kind() const override {
    return NodeType::Program;
  }
};

struct Expr : public Statement {
  // Nothing here yet :p
};

// Getting variables
struct VariableDeclaration : public Statement {
  bool constant;
  std::string identifier;
  // Optional means that it can be undefined, such as `int x;`
  std::optional<std::shared_ptr<Expr>> value;

  VariableDeclaration(bool c, std::string id, std::optional<std::shared_ptr<Expr>> val)
    : constant(c), identifier(std::move(id)), value(std::move(val)) {}

  NodeType kind() const override {
    return NodeType::VarDeclaration;
  }
};

// Getting Binary Expressions
struct BinaryExpr : public Expr {
  NodeType kind() const override {
    return NodeType::BinaryExpr;
  }

  std::shared_ptr<Expr> left;
  std::shared_ptr<Expr> right;
  std::string op;
};

// Getting identifiers
struct Identifier : public Expr {
  NodeType kind() const override {
    return NodeType::Identifier;
  }

  std::string symbol;
};

// Getting numerals (all of them are double cause double is da goat)
struct NumericLiteral : public Expr {
  NodeType kind() const override {
    return NodeType::NumericLiteral;
  }

  double value;

  NumericLiteral(double v) : value(v) {}
};