#include "right_paren_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> RightParenToken::CreateToken(
    std::string str) {
  assert(str == ")"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<RightParenToken>(new RightParenToken());
};

RightParenToken::RightParenToken(){};

const std::string RightParenToken::GetValue() { return ")"; };
}  // namespace token
