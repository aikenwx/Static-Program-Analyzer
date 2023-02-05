#pragma once

#include <string>

#include "token.h"

namespace token {
class NotToken : public SymbolToken {
 public:
  static const NotToken* createToken(std::string);
  const std::string getValue() override;

 private:
  NotToken();
  static NotToken* instance_;
};
}  // namespace token
