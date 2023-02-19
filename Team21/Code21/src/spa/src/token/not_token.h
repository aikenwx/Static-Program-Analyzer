#pragma once

#include <string>

#include "token.h"

namespace token {
class NotToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::NOT; }
  const std::string GetValue() override;

 private:
  NotToken();
};
}  // namespace token
