#pragma once

#include <string>

#include "token.h"

namespace token {
class PlusToken : public Token {
 public:
  static const std::unique_ptr<Token> createToken(std::string);
  const std::string GetValue() override;

 private:
  PlusToken();
};
}  // namespace token
