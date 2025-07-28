#pragma once
#include "environment.h"
#include "values.h"
#include "../frontend/ast.h"
#include "eval/expressions.h"
#include "eval/statements.h"
#include <memory>
#include <stdexcept>
#include <cmath>

// -------- EVALUATING -------
inline std::shared_ptr<RuntimeVal> evaluate(std::shared_ptr<Statement> astNode, std::shared_ptr<Environment> environment){
  switch(astNode->kind()){
    // HANDLING THE PROGRAM
    case NodeType::Program: {
      auto prog = std::dynamic_pointer_cast<Program>(astNode);
      if(!prog) throw std::runtime_error("Internal: Program cast failded");

      std::shared_ptr<RuntimeVal> last = std::make_shared<NullVal>();
      for(auto& statement : prog->body){
        last = evaluate(statement, environment);
      }

      return last;
    }
    // HANDLING STRINGS
    case NodeType::StringLiteral: {
      auto string = std::dynamic_pointer_cast<StringLiteral>(astNode);
      if(!string) {
        throw std::runtime_error("Error: Node isn't a valid StringLiteral.");
      }

      return std::make_shared<StringVal>(string->value);
    }
    // HANDLING NUMERALS
    case NodeType::NumericLiteral: {
      auto num = std::dynamic_pointer_cast<NumericLiteral>(astNode);
      if(!num) {
        throw std::runtime_error("Error: Node isn't a valid NumericLiteral.");
      }

      return std::make_shared<NumberVal>(num->value);
    }
    // HANDLING IDENTIFIERS (var names, etc)
    case NodeType::Identifier: {
      auto ident = std::dynamic_pointer_cast<Identifier>(astNode);
      if(!ident) {
        throw std::runtime_error("Error: Node isn't a valid identifier.");
      }

      return evaluate_identifier(ident, environment);
    }
    // HANDLING OBJECTS
    case NodeType::ObjectLiteral: {
      auto obj = std::dynamic_pointer_cast<ObjectLiteral>(astNode);
      if(!obj){
        throw std::runtime_error("Error: Node isn't a valid ObjectLiteral.");
      }

      return evaluate_objects_expr(obj, environment);
    }
    // HANDLING CALLS
    case NodeType::CallExpr: {
      auto obj = std::dynamic_pointer_cast<CallExpr>(astNode);
      if(!obj){
        throw std::runtime_error("Error: Node isn't a valid ObjectLiteral.");
      }

      return evaluate_call_expr(obj, environment);
    }
    // HANDLING ASSIGNMENTS
    case NodeType::AssignmentExpr: {
      auto assigne = std::dynamic_pointer_cast<AssignmentExpr>(astNode);
      if(!assigne) {
        throw std::runtime_error("Error: Node isn't a valid assignment expression.");
      }

      return evaluate_assignment(assigne, environment);
    }
    // HANDLING BINARY EXPRESSIONS (1 + 1)
    case NodeType::BinaryExpr: {
      return evaluate_binary_expr(astNode, environment);
    }
    // HANDLE STATEMENTS
    case NodeType::VarDeclaration: {
      return evaluate_var_declaration(astNode, environment);
    }
    // HANDLE FUNCTION DECLARATIONS
    case NodeType::FunctionDeclaration: {
      return evaluate_function_declaration(astNode, environment);
    }

    default: {
      std::cout << astNode;
      throw std::runtime_error("Error: unrecognized AST Node");
      exit(1);
    }
  }
}
