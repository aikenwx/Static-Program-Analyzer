#pragma once

#include <string>

#include "token.h"

namespace token {
class NotToken : public Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  const std::string GetValue() override;

 private:
  NotToken();
};
}  // namespace token
