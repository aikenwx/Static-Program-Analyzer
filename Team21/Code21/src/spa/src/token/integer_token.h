#pragma once

#include <string>

#include "token.h"

namespace token {
class IntegerToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::INTEGER; }
  const std::string GetValue() override;

 private:
  IntegerToken(std::string value);
  std::string value;
};
}  // namespace token
