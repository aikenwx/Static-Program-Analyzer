#include <assert.h>
#include <string>

#include "minus_token.h"

namespace token {
const std::unique_ptr<Token> MinusToken::createToken(std::string str) {
  assert(str == "-");
  return std::unique_ptr<MinusToken>(new MinusToken());
};

MinusToken::MinusToken(){};

const std::string MinusToken::getValue() { return "-"; };
}  // namespace token
