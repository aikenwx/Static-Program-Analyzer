#pragma once

#include <string>

#include "token.h"

namespace token {
class IdentifierToken : public Token {
 public:
  static const std::unique_ptr<Token> createToken(std::string);
  const std::string GetValue() override;

 private:
  IdentifierToken(std::string value);
  std::string value;
};
}  // namespace token
