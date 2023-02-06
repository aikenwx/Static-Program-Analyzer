#pragma once

#include <string>

#include "token.h"

namespace token {
class PlusToken : public SymbolToken {
 public:
  static const PlusToken* createToken(std::string);
  const std::string getValue() override;

 private:
  PlusToken();
  static PlusToken* instance_;
};
}  // namespace token
