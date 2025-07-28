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
  AssignmentExpr,
  MemberExpr,
  CallExpr,

  // LITERALS
  Property,
  StringLiteral,
  ObjectLiteral,
  NumericLiteral,
  Identifier,
  BinaryExpr,
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

// Assignment Expressions
// x = 40; after declaration
struct AssignmentExpr : public Expr {
  std::shared_ptr<Expr> assigne;
  std::shared_ptr<Expr> value;

  AssignmentExpr(std::shared_ptr<Expr> ass, std::shared_ptr<Expr> val)
    : assigne(ass), value(val) {}

  NodeType kind() const override {
    return NodeType::AssignmentExpr;
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

// Getting Call Expressions
struct CallExpr : public Expr {
  NodeType kind() const override {
    return NodeType::CallExpr;
  }

  std::vector<std::shared_ptr<Expr>> args;
  std::shared_ptr<Expr> calle;

  CallExpr(std::shared_ptr<Expr> caller, std::vector<std::shared_ptr<Expr>> arg)
    : args(arg), calle(caller) {}
};

// Getting Member Expressions
struct MemberExpr : public Expr {
  NodeType kind() const override {
    return NodeType::MemberExpr;
  }

  std::shared_ptr<Expr> object;
  std::shared_ptr<Expr> property;
  bool computed;

  MemberExpr(std::shared_ptr<Expr> obj, std::shared_ptr<Expr> prop, bool comp)
    : object(obj), property(prop), computed(comp) {}
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

struct StringLiteral : public Expr {
  NodeType kind() const override {
    return NodeType::StringLiteral;
  }

  std::string value;

  StringLiteral(std::string v) : value(v) {}
};

// Property hehee
struct Property : public Expr {
  NodeType kind() const override {
    return NodeType::Property;
  }

  std::string key;
  std::optional<std::shared_ptr<Expr>> value;


  Property(std::string k, std::optional<std::shared_ptr<Expr>> val)
  : key(k), value(val) {}

  Property(std::string k)
  : key(k), value(std::nullopt) {}
};

struct ObjectLiteral : public Expr {
  NodeType kind() const override {
    return NodeType::ObjectLiteral;
  }

  std::vector<Property> properties;

  ObjectLiteral(std::vector<Property> props) : properties(props) {}
};

