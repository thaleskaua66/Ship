#pragma once
#include "../environment.h"
#include "../values.h"
#include "../../frontend/ast.h"
#include "../interpreter.h"
#include <charconv>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <string>
#include <functional>

// I have to put it here cause for some reason he doesn't use correctly the evaluate function even after i import it
std::shared_ptr<RuntimeVal> evaluate(std::shared_ptr<Statement> stmt, std::shared_ptr<Environment> env);

// ------ Evaluating binary expressions --------
inline std::shared_ptr<RuntimeVal> evaluate_binary_expr(std::shared_ptr<Statement> binop, std::shared_ptr<Environment> env) {
  auto binExpr = std::dynamic_pointer_cast<BinaryExpr>(binop);
  if(!binExpr) {
    throw std::runtime_error("Error: Node isn't a valid BinaryExpr.");
  }

  auto left = evaluate(binExpr->left, env);
  auto right = evaluate(binExpr->right, env);

  if(left->type != ValueType::NUMBER || right->type != ValueType::NUMBER){
    throw std::runtime_error("Error: only numbers are supported for binary expressions.");
  }

  auto leftVal = std::dynamic_pointer_cast<NumberVal>(left)->value;
  auto rightVal = std::dynamic_pointer_cast<NumberVal>(right)->value;
  if(!leftVal || !rightVal) throw std::runtime_error("Error: Invalid number values for binary expression.");
  double result;


  // TODO: nothing, it's perfect
  if(binExpr->op == "+") result = leftVal + rightVal;
  else if(binExpr->op == "-") result = leftVal - rightVal;
  else if(binExpr->op == "*") result = leftVal * rightVal;
  else if(binExpr->op == "/") result = leftVal / rightVal;
  else if(binExpr->op == "%") result = std::fmod(leftVal, rightVal);
  else throw std::runtime_error("Error: Invalid operator.");

  return std::make_shared<NumberVal>(result);
}

// Evaluating identifiers
inline std::shared_ptr<RuntimeVal> evaluate_identifier(std::shared_ptr<Identifier> ident, std::shared_ptr<Environment> env){
  std::shared_ptr<RuntimeVal> value = env->lookupVar(ident->symbol);
  return value;
}

inline std::shared_ptr<RuntimeVal> evaluate_objects_expr(std::shared_ptr<ObjectLiteral> obj, std::shared_ptr<Environment> env){
  auto object = std::make_shared<ObjectVal>();

  for(auto& prop : obj->properties){
    std::string key = prop.key;

    std::shared_ptr<RuntimeVal> value;

    if(prop.value.has_value()){
      value = evaluate(prop.value.value(), env);
    } else {
      value = env->lookupVar(key);
    }

    object->properties[key] = value;
  }

  return object;
}

// 
inline std::shared_ptr<RuntimeVal> evaluate_call_expr(std::shared_ptr<CallExpr> expr, std::shared_ptr<Environment> env){
  std::vector<std::shared_ptr<RuntimeVal>> args;
  for(auto& arg : expr->args){
    args.push_back(evaluate(arg, env));
  }

  auto fn = evaluate(expr->calle, env);

  if(fn->type != ValueType::NATIVEFUNCTION){
    throw std::runtime_error("Cannot call value that is not a function.");
    exit(1);
  }

  auto fnVal = std::static_pointer_cast<NativeFunctionVal>(fn);
  auto result = fnVal->call(args, env);

  return result;
}

// Evaluating assignments
inline std::shared_ptr<RuntimeVal> evaluate_assignment(std::shared_ptr<AssignmentExpr> node, std::shared_ptr<Environment> env){
  if(node->assigne->kind() != NodeType::Identifier){
    throw std::runtime_error("Invalid LHS inside assignment expression.");
  }
  std::string varName = std::dynamic_pointer_cast<Identifier>(node->assigne)->symbol;
  return env->assignVar(varName, evaluate(node->value, env));
}