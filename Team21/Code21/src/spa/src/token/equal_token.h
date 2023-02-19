#pragma once

#include <string>

#include "token.h"

namespace token {
class EqualToken : public Token {
 public:
  static const std::unique_ptr<Token> createToken(std::string);
  const std::string GetValue() override;

 private:
  EqualToken();
};
}  // namespace token
