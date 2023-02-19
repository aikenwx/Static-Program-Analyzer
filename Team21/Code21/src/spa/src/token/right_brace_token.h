#pragma once

#include <string>

#include "token.h"

namespace token {
class RightBraceToken : public Token {
 public:
  static const std::unique_ptr<Token> createToken(std::string);
  const std::string GetValue() override;

 private:
  RightBraceToken();
};
}  // namespace token
