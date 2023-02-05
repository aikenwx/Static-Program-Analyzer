#pragma once

#include <string>

#include "token.h"

namespace token {
class RightParenToken : public SymbolToken {
 public:
  static const RightParenToken* createToken(std::string);
  const std::string getValue() override;
 private:
  RightParenToken();
  static RightParenToken* instance_;
};
}  // namespace token
