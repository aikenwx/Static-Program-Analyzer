#pragma once

#include <string>

#include "token.h"

namespace token {
class DivideToken : public SymbolToken {
 public:
  static const DivideToken* createToken(std::string);
  const std::string getValue() override;

 private:
  DivideToken();
  static DivideToken* instance_;
};
}  // namespace token
