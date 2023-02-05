#pragma once

#include <string>

#include "token.h"

namespace token {
class ModuloToken : public SymbolToken {
 public:
  static const ModuloToken* createToken(std::string);
  const std::string getValue() override;
 private:
  ModuloToken();
  static ModuloToken* instance_;
};
}  // namespace token
