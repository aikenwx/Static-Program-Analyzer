#pragma once

#include <string>

#include "token.h"

namespace token {
class IdentifierToken : public Token {
 public:
  static const IdentifierToken* createToken(std::string);
  const std::string getValue() override;

 private:
  IdentifierToken(std::string value);
  std::string value;
};
}  // namespace token