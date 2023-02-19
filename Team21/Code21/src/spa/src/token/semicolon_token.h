#pragma once

#include <string>

#include "token.h"

namespace token {
class SemicolonToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::SEMICOLON; }
  const std::string GetValue() override;

 private:
  SemicolonToken();
};
}  // namespace token
