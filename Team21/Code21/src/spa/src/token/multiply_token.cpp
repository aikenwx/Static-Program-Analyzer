#include "multiply_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> MultiplyToken::createToken(std::string str) {
  assert(str == "*");
  return std::unique_ptr<MultiplyToken>(new MultiplyToken());
};

MultiplyToken::MultiplyToken(){};

const std::string MultiplyToken::getValue() { return "*"; };
}  // namespace token
