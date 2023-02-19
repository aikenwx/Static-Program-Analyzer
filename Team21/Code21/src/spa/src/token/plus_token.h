#pragma once

#include <string>

#include "token.h"

namespace token {
class PlusToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::PLUS; }
  const std::string GetValue() override;

 private:
  PlusToken();
};
}  // namespace token
