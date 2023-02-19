#pragma once

#include <string>

#include "token.h"

namespace token {
/**
 * Special token denoting end of input, used in parser
 */
class EndToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const TokenType GetType() override { return TokenType::END; }
  const std::string GetValue() override;

 private:
  EndToken();
};
}  // namespace token
