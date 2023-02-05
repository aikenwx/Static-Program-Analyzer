#pragma once

#include <string>

#include "token.h"

namespace token {
class EqualToken : public SymbolToken {
 public:
  static const EqualToken* createToken(std::string);
  const std::string getValue() override;

 private:
  EqualToken();
  static EqualToken* instance_;
};
}  // namespace token
