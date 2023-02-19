#pragma once

#include <string>

#include "token.h"

namespace token {
class LeftBraceToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::LEFT_BRACE; }
  const std::string GetValue() override;

 private:
  LeftBraceToken();
};
}  // namespace token
