#pragma once
#include "ast.h"
#include "lexer.h"
#include <memory>
#include <stdexcept>
#include <vector>
#include <string>
#include <map>

// Printing
inline std::ostream& operator<<(std::ostream& os, const Token& token) {
    std::string typeName;
    switch(token.type) {
        case TokenType::Number: typeName = "Number"; break;
        case TokenType::Identifier: typeName = "Identifier"; break;
        case TokenType::Equals: typeName = "Equals"; break;
        case TokenType::OpenParen: typeName = "OpenParen"; break;
        case TokenType::CloseParen: typeName = "CloseParen"; break;
        case TokenType::BinaryOperator: typeName = "BinaryOperator"; break;
        case TokenType::Yarr: typeName = "Yarr"; break;
        case TokenType::eof: typeName = "EOF"; break;
        default: typeName = "Unknown"; break;
    }
    os << "Token(" << typeName << ", \"" << token.value << "\")";
    return os;
}

class Parser {
  private:
    std::vector<Token> tokens;
    size_t current = 0;

    bool not_eof(){
      return current < tokens.size() && tokens[current].type != TokenType::eof;
    }

    Token advance() {
      return this->tokens[current++];
    }

    Token at() const{
      return this->tokens[current];
    }

    Token expect(TokenType type, std::string err){
      Token prev = at();
      if(prev.type != type){
        std::cout << "Parser Error:\n" << err << '\n' << prev << std::endl;
        exit(1);
      }

      return advance();
    }

    std::shared_ptr<Statement> parseStatement() {
      // skip to parse
      switch(at().type){
        case TokenType::Yarr: {
          return this->parse_var_declaration();
        }
        case TokenType::Grrr: {
          return this->parse_var_declaration();
        }
        case TokenType::Map: {
          return this->parse_function_declaration();
        }
        default:
          return this->parse_statement_expr();
      }
    }

    // Parsing function declarations
    std::shared_ptr<Statement> parse_function_declaration(){
      advance(); // eat Map keyword
      Token name = expect(TokenType::Identifier, "Expected function name following map keyword.");
      auto args = this->parse_args();
      std::vector<std::string> params;
      for(std::shared_ptr<Expr> arg : args){
        if(arg->kind() != NodeType::Identifier){
          throw std::runtime_error("Parameter should be string type inside function declaration.");
          exit(1);
        }

        std::shared_ptr<Identifier> id = std::dynamic_pointer_cast<Identifier>(arg);
        if(!id){
          throw std::runtime_error("Parameter is not an identifier.");
          exit(1);
        }

        params.push_back(id->symbol);
      }

      expect(TokenType::OpenBrace, "Expected opening brace for function declaration.");
      std::vector<std::shared_ptr<Statement>> body;

      while(at().type != TokenType::eof && at().type != TokenType::CloseBrace){
        body.push_back(this->parseStatement());
      }

      expect(TokenType::CloseBrace, "Expected closing brace in function end.");
      return std::make_shared<FunctionDeclaration>(params, name.value, body);
    }

    // We always want semicolons to keep the language consistent and not strange
    std::shared_ptr<Statement> parse_statement_expr(){
      auto expr = this->parse_expr();
      this->expect(TokenType::Semicolon, "Expected ';' after expression.");
      return expr;
    }

    std::shared_ptr<Statement> parse_var_declaration(){
      // The isConstant is inverted for some reason lol, had to change to "Yarr", hopefully it doesn't fuck with something else :p
      bool isConstant = advance().type == TokenType::Yarr;
      std::string identifier = expect(TokenType::Identifier, "Expected identifier name following yarr | grrr keywords.").value;

      // declaration verifications
      if(at().type == TokenType::Semicolon) {
        advance();
        if(!isConstant) {
          throw std::runtime_error("Must assign value to constant expression.");
        }

        return std::make_shared<VariableDeclaration>(false, identifier, std::nullopt);
      }

      expect(TokenType::Equals, "Expected equals token identifier in var declaration.");
      auto declaration = std::make_shared<VariableDeclaration>(isConstant, identifier, parse_expr());
      expect(TokenType::Semicolon, "Variable declaration without semicolon.");

      return declaration;
    }

    std::shared_ptr<Expr> parse_expr(){
      return parse_assignment_expr();
    }

    std::shared_ptr<Expr> parse_assignment_expr(){
      std::shared_ptr<Expr> left = this->parse_object_expr();

      if(at().type == TokenType::Equals){
        advance(); // advance past equal;
        auto value = this->parse_assignment_expr(); // Allow chaining
        return std::make_shared<AssignmentExpr>(left, value);
      }

      return left;
    }

    std::shared_ptr<Expr> parse_object_expr(){
      if(at().type != TokenType::OpenBrace){
        return this->parse_additive_expr();
      }

      advance();
      std::vector<Property> properties;

      while(this->not_eof() && at().type != TokenType::CloseBrace){
        auto key = expect(TokenType::Identifier, "Object literal key expected.").value;

        if(at().type == TokenType::Comma){
          advance();
          properties.push_back(Property{ key });
          continue;
        }
        else if(at().type == TokenType::CloseBrace){
          properties.push_back(Property{ key });
          continue;
        }

        // { key: value }
        expect(TokenType::Colon, "Missing colon following identifier in ObjectExpr.");
        std::shared_ptr<Expr> value = parse_expr();
        properties.push_back(Property{key, value});

        if(at().type != TokenType::CloseBrace){
          expect(TokenType::Comma, "Expected comma or closing brace.");
        }
      }

      expect(TokenType::CloseBrace, "Object literal missing closing brace.");
      return std::make_shared<ObjectLiteral>(properties);
    }

    std::shared_ptr<Expr> parse_additive_expr(){
      std::shared_ptr<Expr> left = this->parse_multiplicative_expr();

      while(this->at().value == "+" || this->at().value == "-"){
        std::string op = this->advance().value;
        std::shared_ptr<Expr> right = this->parse_multiplicative_expr();
        auto binop = std::make_shared<BinaryExpr>();
        binop->left = left;
        binop->right = right;
        binop->op = op;

        left = binop;
      }

      return left;
    }

    std::shared_ptr<Expr> parse_multiplicative_expr(){
      std::shared_ptr<Expr> left = this->parse_call_member_expr();

      while(this->at().value == "*" || this->at().value == "/" || this->at().value == "%"){
        std::string op = this->advance().value;
        std::shared_ptr<Expr> right = this->parse_call_member_expr();
        auto binop = std::make_shared<BinaryExpr>();
        binop->left = left;
        binop->right = right;
        binop->op = op;

        left = binop;
      }

      return left;
    }

    std::shared_ptr<Expr> parse_call_member_expr(){
      std::shared_ptr<Expr> member = this->parse_member_expr();

      if(at().type == TokenType::OpenParen){
        return this->parse_call_expr(member);
      }

      return member;
    }

    std::shared_ptr<Expr> parse_call_expr(std::shared_ptr<Expr> calle){
      std::shared_ptr<Expr> call_expr = std::make_shared<CallExpr>(calle, this->parse_args());

      if(at().type == TokenType::OpenParen){
        call_expr = this->parse_call_expr(call_expr);
      }

      return call_expr;
    }

    std::vector<std::shared_ptr<Expr>> parse_args(){
      expect(TokenType::OpenParen, "Expected open parenthesis.");
      auto args = at().type == TokenType::CloseParen ? std::vector<std::shared_ptr<Expr>>{} : this->parse_args_list();

      expect(TokenType::CloseParen, "Missing closing parenthesis inside argument list.");
      return args;
    }

    std::vector<std::shared_ptr<Expr>> parse_args_list(){
      std::vector<std::shared_ptr<Expr>> args;
      args.push_back(this->parse_assignment_expr());

      while(this->at().type == TokenType::Comma){
        advance();
        args.push_back(this->parse_assignment_expr());
      }

      return args;
    }

    std::shared_ptr<Expr> parse_member_expr(){
      std::shared_ptr<Expr> object = this->parse_primary_expr();

      while(at().type == TokenType::Dot || at().type == TokenType::OpenBracket){
        Token op = advance();
        std::shared_ptr<Expr> property;
        bool computed;

        // non-computed value (obj.expr)
        if(op.type == TokenType::Dot){
          computed = false;
          // Getting identifier as property
          property = this->parse_primary_expr();

          if(property->kind() != NodeType::Identifier){
            throw std::runtime_error("Cannot use dot operator without right hand side being a identifier.");
            // exit(1);
          }
        } else { // This allows chaining hell yarr!
          computed = true;
          property = this->parse_expr();
          expect(TokenType::CloseBracket, "Missing closing bracket in computed value.");
        }

        object = std::make_shared<MemberExpr>(object, property, computed);
      }

      return object;
    }

    std::shared_ptr<Expr> parse_primary_expr(){
      Token tk = this->at();
      TokenType type = tk.type;

      switch(tk.type){
        case TokenType::Identifier: {
          advance();
          auto id = std::make_shared<Identifier>();
          id->symbol = tk.value;
          return id;
        }
        case TokenType::String: {
          advance();
          auto string = std::make_shared<StringLiteral>(tk.value);
          string->value = tk.value;
          return string;
        }
        case TokenType::Number: {
          advance();
          auto num = std::make_shared<NumericLiteral>(10);
          num->value = std::stod(tk.value);
          return num;
        }
        case TokenType::OpenParen: {
          advance(); // skips the opening paren
          std::shared_ptr<Expr> value = this->parse_expr();
          expect(
            TokenType::CloseParen,
            "You forgot to close a parenthesis."
          ); // closing paren
          return value;
        }
          
        default:
          std::cerr << "Error: invalid primary expression: " << tk.value << std::endl;
          exit(1);
      }
    }

  public:
    std::shared_ptr<Program> produceAST(const std::string& sourceCode) {
      tokens = tokenize(sourceCode);
      current = 0;

      auto program = std::make_shared<Program>();

      // Parsing :3
      while(this->not_eof()){
        program->body.push_back(parseStatement());
      }

      return program;
    };

    
};