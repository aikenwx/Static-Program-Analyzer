#pragma once

#include <string>

#include "token.h"

namespace token {
class DivideToken : public Token {
 public:
  static const std::unique_ptr<Token> createToken(std::string);
  const std::string getValue() override;

 private:
  DivideToken();
};
}  // namespace token
