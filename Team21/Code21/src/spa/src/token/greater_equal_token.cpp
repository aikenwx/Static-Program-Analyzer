#include <assert.h>
#include <string>

#include "greater_equal_token.h"

namespace token {
GreaterEqualToken::GreaterEqualToken(){};

const std::string GreaterEqualToken::getValue() { return ">="; };

const GreaterEqualToken* GreaterEqualToken::createToken(std::string str) {
  assert(str == ">=");
  if (instance_ == nullptr) instance_ = new GreaterEqualToken();
  return instance_;
};

GreaterEqualToken* GreaterEqualToken::instance_ = nullptr;
}  // namespace token
