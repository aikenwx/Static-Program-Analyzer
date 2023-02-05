#include "less_than_token.h"

#include <assert.h>

#include <string>

namespace token {
LessThanToken::LessThanToken(){};

const std::string LessThanToken::getValue() { return "<"; };

const LessThanToken* LessThanToken::createToken(std::string str) {
  assert(str == "<");
  return new LessThanToken();
};
}  // namespace token