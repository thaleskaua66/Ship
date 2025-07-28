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
  Comma, Colon, Dot,
  Semicolon,
  OpenParen, CloseParen, // ()
  OpenBrace, CloseBrace, // {}
  OpenBracket, CloseBracket, // []
  String, // ""
  BinaryOperator,
  eof, // end of file 

  // Keywords
  Yarr,
  Grrr,
  Map,
};

inline std::map<std::string, TokenType> keywords = {
  {"yarr", TokenType::Yarr},
  {"grrr", TokenType::Grrr},
  {"map", TokenType::Map},
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

    if(current == ' ' || current == '\n' || current == '\t' || current == '\r') continue;

    switch(current){
      case '(': tokens.push_back({"(", TokenType::OpenParen}); continue;
      case ')': tokens.push_back({")", TokenType::CloseParen}); continue;
      case '{': tokens.push_back({"{", TokenType::OpenBrace}); continue;
      case '}': tokens.push_back({"}", TokenType::CloseBrace}); continue;
      case '[': tokens.push_back({"[", TokenType::OpenBracket}); continue;
      case ']': tokens.push_back({"]", TokenType::CloseBracket}); continue;
      case '+':
      case '-':
      case '/':
      case '*':
      case '%':
        tokens.push_back({std::string(1, current), TokenType::BinaryOperator});
        continue;
      case '=': tokens.push_back({"=", TokenType::Equals}); continue;
      case ';': tokens.push_back({";", TokenType::Semicolon}); continue;
      case ':': tokens.push_back({":", TokenType::Colon}); continue;
      case ',': tokens.push_back({",", TokenType::Comma}); continue;
      case '.': tokens.push_back({".", TokenType::Dot}); continue;
      // HANDLING MULTICHAR TOKENS IN LIGHTNING SPEED
      default:
        if(sourceCode[i] == '"'){
          std::string str = "";

          i++; // To enter in the string
          while(i < sourceCode.length() && sourceCode[i] != '"'){
            if(sourceCode[i] == '\\'){
              i++;
              char next = sourceCode[i];

              switch(next){
                case 'n': str += '\n'; break;
                case 't': str += '\t'; break;
                case '\\': str += '\\'; break;
                case '"': str += '\"'; break;
                case 'r': str += '\r'; break;
                case '0': str += '\0'; break;
                default:
                  std::cerr << "Invalid escape sequence: \\" << next << std::endl;
                  exit(1);
              }

              i++;
            } else {
              str += sourceCode[i++];
            }
          }

          if(i >= sourceCode.length() || sourceCode[i] != '"'){
            std::cerr << "Lexer Error: Unterminated string literal.\n";
            exit(1);
          }

          tokens.push_back({str, TokenType::String});
          continue;
        }
        else if(isdigit(current) || sourceCode[i] == '.'){
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