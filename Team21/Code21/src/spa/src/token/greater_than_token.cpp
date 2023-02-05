#include <assert.h>
#include <string>

#include "greater_than_token.h"

namespace token {
GreaterThanToken::GreaterThanToken(){};

const std::string GreaterThanToken::getValue() { return ">"; };

const GreaterThanToken* GreaterThanToken::createToken(std::string str) {
  assert(str == ">");
  return new GreaterThanToken();
};
}  // namespace token