#include "right_paren_token.h"

#include <assert.h>

#include <string>

namespace token {
RightParenToken::RightParenToken(){};

const std::string RightParenToken::getValue() { return ")"; };

const RightParenToken* RightParenToken::createToken(std::string str) {
  assert(str == ")");
  return new RightParenToken();
};
}  // namespace token