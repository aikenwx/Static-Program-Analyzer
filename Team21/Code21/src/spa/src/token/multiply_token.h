#pragma once

#include <string>

#include "token.h"

namespace token {
class MultiplyToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::MULTIPLY; }
  const std::string GetValue() override;

 private:
  MultiplyToken();
};
}  // namespace token
