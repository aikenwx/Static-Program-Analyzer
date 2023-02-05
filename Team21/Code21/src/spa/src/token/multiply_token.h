#pragma once

#include <string>

#include "token.h"

namespace token {
class MultiplyToken : public SymbolToken {
 public:
  static const MultiplyToken* createToken(std::string);
  const std::string getValue() override;

 private:
  MultiplyToken();
  static MultiplyToken* instance_;
};
}  // namespace token
