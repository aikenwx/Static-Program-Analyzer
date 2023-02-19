#pragma once

#include <string>

#include "token.h"

namespace token {
class GreaterEqualToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::GREATER_EQUAL; }
  const std::string GetValue() override;

 private:
  GreaterEqualToken();
};
}  // namespace token
