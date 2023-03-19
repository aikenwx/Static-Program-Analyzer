#pragma once

#include <string>

#include "token.h"

namespace token {
class RightParenToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::RIGHT_PAREN; }
  const std::string GetValue() override;

 private:
  RightParenToken();
};
}  // namespace token
