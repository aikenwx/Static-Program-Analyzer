#pragma once

#include <string>

#include "token.h"

namespace token {
class GreaterEqualToken : public SymbolToken {
 public:
  static const GreaterEqualToken* createToken(std::string);
  const std::string getValue() override;

 private:
  GreaterEqualToken();
  static GreaterEqualToken* instance_;
};
}  // namespace token