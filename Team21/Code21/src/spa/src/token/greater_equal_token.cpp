#include <assert.h>
#include <string>

#include "greater_equal_token.h"

namespace token {
GreaterEqualToken::GreaterEqualToken(){};

const std::string GreaterEqualToken::getValue() { return ">="; };

const GreaterEqualToken* GreaterEqualToken::createToken(std::string str) {
  assert(str == ">=");
  return new GreaterEqualToken();
};
}  // namespace token
