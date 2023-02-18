#pragma once

#include <string>

#include "token.h"

namespace token {
class RightParenToken : public Token {
 public:
  static const std::unique_ptr<Token> createToken(std::string);
  const std::string getValue() override;

 private:
  RightParenToken();
};
}  // namespace token
