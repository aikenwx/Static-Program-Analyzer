#include <assert.h>
#include <string>

#include "minus_token.h"

namespace token {
MinusToken::MinusToken(){};

const std::string MinusToken::getValue() { return "-"; };

const MinusToken* MinusToken::createToken(std::string str) {
  assert(str == "-");
  if (instance_ == nullptr) instance_ = new MinusToken();
  return instance_;
};

MinusToken* MinusToken::instance_ = nullptr;
}  // namespace token
