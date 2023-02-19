#pragma once

#include <string>

#include "token.h"

namespace token {
class GreaterThanToken : public Token {
 public:
  static const std::unique_ptr<Token> createToken(std::string);
  const std::string GetValue() override;

 private:
  GreaterThanToken();
};
}  // namespace token
