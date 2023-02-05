#pragma once

#include <string>

#include "token.h"

namespace token {
class LeftParenToken : public SymbolToken {
 public:
  static const LeftParenToken* createToken(std::string);
  const std::string getValue() override;

 private:
  LeftParenToken();
  static LeftParenToken* instance_;
};
}  // namespace token
