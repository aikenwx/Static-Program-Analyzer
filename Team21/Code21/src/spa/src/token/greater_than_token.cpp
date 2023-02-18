#include <assert.h>
#include <string>

#include "greater_than_token.h"

namespace token {
const std::unique_ptr<Token> GreaterThanToken::createToken(std::string str) {
  assert(str == ">");
  return std::unique_ptr<GreaterThanToken>(new GreaterThanToken());
};

GreaterThanToken::GreaterThanToken(){};

const std::string GreaterThanToken::getValue() { return ">"; };
}  // namespace token
