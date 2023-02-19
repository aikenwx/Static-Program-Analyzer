#pragma once

#include <string>

#include "token.h"

namespace token {
class AssignToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::ASSIGN; }
  const std::string GetValue() override;

 private:
  AssignToken();
};
}  // namespace token
