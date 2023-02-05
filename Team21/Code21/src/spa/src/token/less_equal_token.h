#pragma once

#include <string>

#include "token.h"

namespace token {
class LessEqualToken : public SymbolToken {
 public:
  static const LessEqualToken* createToken(std::string);
  const std::string getValue() override;
 private:
  LessEqualToken();
  static LessEqualToken* instance_;
};
}  // namespace token