#pragma once

#include <string>

#include "token.h"

namespace token {
class LeftParenToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::LEFT_PAREN; }
  const std::string GetValue() override;

 private:
  LeftParenToken();
};
}  // namespace token
