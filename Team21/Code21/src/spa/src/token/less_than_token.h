#pragma once

#include <string>

#include "token.h"

namespace token {
class LessThanToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::LESS_THAN; }
  const std::string GetValue() override;

 private:
  LessThanToken();
};
}  // namespace token
