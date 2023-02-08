#pragma once

#include <string>

#include "token.h"

namespace token {
class AssignToken : public SymbolToken {
 public:
  static const AssignToken* createToken(std::string);
  const std::string getValue() override;

 private:
  AssignToken();
  static AssignToken* instance_;
};
}  // namespace token
