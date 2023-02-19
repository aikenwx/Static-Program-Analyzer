#pragma once
#include <memory>
#include <string>

namespace token {
enum TokenType {
  AND,
  ASSIGN,
  DIVIDE,
  EQUAL,
  GREATER_EQUAL,
  GREATER_THAN,
  IDENTIFIER,
  INTEGER,
  LEFT_BRACE,
  LEFT_PAREN,
  LESS_EQUAL,
  LESS_THAN,
  MINUS,
  MODULO,
  MULTIPLY,
  NOT_EQUAL,
  NOT,
  OR,
  PLUS,
  RIGHT_BRACE,
  RIGHT_PAREN,
  SEMICOLON,
};

class Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  virtual ~Token(){};
  virtual const TokenType GetType() = 0;
  virtual const std::string GetValue() = 0;
};
}  // namespace token
