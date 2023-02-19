#pragma once

#include <string>

#include "token.h"

namespace token {
class ModuloToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::MODULO; }
  const std::string GetValue() override;

 private:
  ModuloToken();
};
}  // namespace token
