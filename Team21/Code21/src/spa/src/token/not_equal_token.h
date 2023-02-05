#pragma once

#include <string>

#include "token.h"

namespace token {
class NotEqualToken : public SymbolToken {
 public:
  static const NotEqualToken* createToken(std::string);
  const std::string getValue() override;
 private:
  NotEqualToken();
  static NotEqualToken* instance_;
};
}  // namespace token
