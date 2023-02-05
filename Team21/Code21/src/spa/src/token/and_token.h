#pragma once

#include <string>

#include "token.h"

namespace token {
class AndToken : public SymbolToken {
 public:
  static const AndToken* createToken(std::string);
  const std::string getValue() override;

 private:
  AndToken();
  static AndToken* instance_;
};
}  // namespace token