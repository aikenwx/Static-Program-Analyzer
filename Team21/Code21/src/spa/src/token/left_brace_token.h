#pragma once

#include <string>

#include "token.h"

namespace token {
class LeftBraceToken : public SymbolToken {
 public:
  static const LeftBraceToken* createToken(std::string);
  const std::string getValue() override;

 private:
  LeftBraceToken();
  static LeftBraceToken* instance_;
};
}  // namespace token