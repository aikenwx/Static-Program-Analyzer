#include <assert.h>
#include <string>

#include "minus_token.h"

namespace token {
MinusToken::MinusToken(){};

const std::string MinusToken::getValue() { return "-"; };

const MinusToken* MinusToken::createToken(std::string str) {
  assert(str == "-");
  return new MinusToken();
};
}  // namespace token