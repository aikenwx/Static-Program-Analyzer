#pragma once

#include <string>

#include "token.h"

namespace token {
class MinusToken : public SymbolToken {
 public:
  static const MinusToken* createToken(std::string);
  const std::string getValue() override;

 private:
  MinusToken();
  static MinusToken* instance_;
};
}  // namespace token
