#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <map>

// Var Types
enum class TokenType {
  // Literals
  Number,
  Identifier,

  // Grouping & operators
  Equals,
  Semicolon,
  OpenParen, CloseParen,
  BinaryOperator,
  eof, // end of file 

  // Keywords
  Yarr,
  Grrr,
};

inline std::map<std::string, TokenType> keywords = {
  {"yarr", TokenType::Yarr},
  {"grrr", TokenType::Grrr},
};

struct Token {
  std::string value;
  TokenType type;
};

// --------- tokenizer
inline std::vector<Token> tokenize(std::string sourceCode){
  std::vector<Token> tokens;

  for(int i = 0; i < sourceCode.length(); i++) {
    char current = sourceCode[i];

    if(current == ' ' || current == '\n' || current == '\t') continue;

    switch(current){
      case '(': tokens.push_back({"(", TokenType::OpenParen}); continue;
      case ')': tokens.push_back({")", TokenType::CloseParen}); continue;
      case '+':
      case '-':
      case '/':
      case '*':
      case '%':
        tokens.push_back({std::string(1, current), TokenType::BinaryOperator});
        continue;
      case '=': tokens.push_back({"=", TokenType::Equals}); continue;
      case ';': tokens.push_back({";", TokenType::Semicolon}); continue;
      // HANDLING MULTICHAR TOKENS IN LIGHTNING SPEED
      default:
        if(isdigit(current) || sourceCode[i] == '.'){
          std::string number;
          bool hasDot;

          while(i < sourceCode.length() && (isdigit(sourceCode[i]) || (sourceCode[i] == '.' && !hasDot))){
            if(sourceCode[i] == '.') hasDot = true;
            number += sourceCode[i++];
          }
          i--; // Because of the for loop
          tokens.push_back({number, TokenType::Number});
          continue;
        }
        else if(isalpha(current) || current == '_'){
          std::string identifier;

          while(i < sourceCode.length() && (isalpha(sourceCode[i]) || sourceCode[i] == '_')){
            identifier += sourceCode[i++];
          }
          i--; // Yes, because of the for loop

          // check for reserved keywords. YARR!!
          auto found = keywords.find(identifier);

          if(found != keywords.end()){
            tokens.push_back({identifier, found->second});
          } else {
            tokens.push_back({identifier, TokenType::Identifier});
          }
          continue;

        } else { // Then it must be an error
          std::cerr << "Invalid identifier: " << current << std::endl;
          exit(1);
        }
    }
  }

  tokens.push_back({"EOF", TokenType::eof});
  return tokens;
}