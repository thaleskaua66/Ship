#pragma once
#include "../environment.h"
#include "../values.h"
#include "../../frontend/ast.h"
#include "../interpreter.h"
#include <memory>
#include <stdexcept>
#include <cmath>

std::shared_ptr<RuntimeVal> evaluate(std::shared_ptr<Statement> stmt, std::shared_ptr<Environment> env);

inline std::shared_ptr<RuntimeVal> evaluate_var_declaration(std::shared_ptr<Statement> var, std::shared_ptr<Environment> env){
  auto declaration = std::dynamic_pointer_cast<VariableDeclaration>(var);
  std::shared_ptr<RuntimeVal> value;

  if(declaration->value.has_value()){
    value = evaluate(declaration->value.value(), env);
  } else {
    value = std::make_shared<NullVal>();
  }

  return env->declareVar(declaration->identifier, value, declaration->constant);
}