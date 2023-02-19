#include "right_paren_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> RightParenToken::createToken(
    std::string str) {
  assert(str == ")");
  return std::unique_ptr<RightParenToken>(new RightParenToken());
};

RightParenToken::RightParenToken(){};

const std::string RightParenToken::GetValue() { return ")"; };
}  // namespace token
