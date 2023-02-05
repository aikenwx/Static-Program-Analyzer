#pragma once

#include <string>

#include "token.h"

namespace token {
class OrToken : public SymbolToken {
 public:
  static const OrToken* createToken(std::string);
  const std::string getValue() override;
 private:
  OrToken();
  static OrToken* instance_;
};
}  // namespace token