#include "left_paren_token.h"

#include <assert.h>

#include <string>

namespace token {
LeftParenToken::LeftParenToken(){};

const std::string LeftParenToken::getValue() { return "("; };

const LeftParenToken* LeftParenToken::createToken(std::string str) {
  assert(str == "(");
  return new LeftParenToken();
};
}  // namespace token