#pragma once

#include <string>

#include "token.h"

namespace token {
class AndToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::AND; }
  const std::string GetValue() override;

 private:
  AndToken();
};
}  // namespace token
