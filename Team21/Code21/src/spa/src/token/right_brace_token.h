#pragma once

#include <string>

#include "token.h"

namespace token {
class RightBraceToken : public SymbolToken {
 public:
  static const RightBraceToken* createToken(std::string);
  const std::string getValue() override;
 private:
  RightBraceToken();
  static RightBraceToken* instance_;
};
}  // namespace token