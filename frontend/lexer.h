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
  size_t i = 0;

  while(i < sourceCode.length()) {
    char current = sourceCode[i];

    if(current == ' ' || current == '\n' || current == '\t'){
      i++;
      continue;
    }

    if(current == '('){
      tokens.push_back({"(", TokenType::OpenParen});
      i++;
      continue;
    } else if(current == ')'){
      tokens.push_back({")", TokenType::CloseParen});
      i++;
      continue;
    } else if(current == '+' || current == '-' || current == '*' || current == '/' || current == '%'){
      tokens.push_back({std::string(1, current), TokenType::BinaryOperator});
      i++;
      continue;
    } else if(current == '='){
      tokens.push_back({"=", TokenType::Equals});
      i++;
      continue;
    } else if(current == ';') {
      tokens.push_back({";", TokenType::Semicolon});
      i++;
      continue;
    } else {
      // Handle multichar tokens :D
      if(isdigit(current)){
        std::string number;

        while(i < sourceCode.length() && isdigit(sourceCode[i])){
          number += sourceCode[i++];
        }
        tokens.push_back({number, TokenType::Number});
        continue;

      } else if(isalpha(current)){
        std::string identifier;

        while(i < sourceCode.length() && isalpha(sourceCode[i])){
          identifier += sourceCode[i++];
        }
        // check for reserved keywords. YARR!!
        auto found = keywords.find(identifier);

        if(found != keywords.end()){
          tokens.push_back({identifier, found->second});
        } else {
          tokens.push_back({identifier, TokenType::Identifier});
        }
        continue;
      } else {
        std::cerr << "Invalid identifier: " << current << std::endl;
        exit(1);
      }
    }
  }

  tokens.push_back({"EOF", TokenType::eof});
  return tokens;
}