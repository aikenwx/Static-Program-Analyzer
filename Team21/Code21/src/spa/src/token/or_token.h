#pragma once

#include <string>

#include "token.h"

namespace token {
class OrToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const std::string GetValue() override;

 private:
  OrToken();
};
}  // namespace token
