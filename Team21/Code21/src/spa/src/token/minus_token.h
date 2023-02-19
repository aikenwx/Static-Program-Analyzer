#pragma once

#include <string>

#include "token.h"

namespace token {
class MinusToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::MINUS; }
  const std::string GetValue() override;

 private:
  MinusToken();
};
}  // namespace token
