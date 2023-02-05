#pragma once

#include <string>

#include "token.h"

namespace token {
class LessThanToken : public SymbolToken {
 public:
  static const LessThanToken* createToken(std::string);
  const std::string getValue() override;
 private:
  LessThanToken();
  static LessThanToken* instance_;
};
}  // namespace token
