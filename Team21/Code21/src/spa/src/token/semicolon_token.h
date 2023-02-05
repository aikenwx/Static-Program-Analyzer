#pragma once

#include <string>

#include "token.h"

namespace token {
class SemicolonToken : public SymbolToken {
 public:
  static const SemicolonToken* createToken(std::string);
  const std::string getValue() override;

 private:
  SemicolonToken();
  static SemicolonToken* instance_;
};
}  // namespace token