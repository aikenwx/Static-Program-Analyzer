#include "multiply_token.h"

#include <assert.h>

#include <string>

namespace token {
MultiplyToken::MultiplyToken() {}

const std::string MultiplyToken::getValue() { return "*"; }

const MultiplyToken* MultiplyToken::createToken(std::string str) {
  assert(str == "*");
  return new MultiplyToken();
}
}  // namespace token