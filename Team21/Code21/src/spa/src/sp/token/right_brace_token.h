#pragma once

#include <string>

#include "token.h"

namespace token {
class RightBraceToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::RIGHT_BRACE; }
  const std::string GetValue() override;

 private:
  RightBraceToken();
};
}  // namespace token
