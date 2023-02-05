#pragma once

#include <string>

#include "token.h"

namespace token {
class GreaterThanToken : public SymbolToken {
 public:
  static const GreaterThanToken* createToken(std::string);
  const std::string getValue() override;

 private:
  GreaterThanToken();
  static GreaterThanToken* instance_;
};
}  // namespace token
