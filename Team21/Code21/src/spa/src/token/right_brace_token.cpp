#include "right_brace_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> RightBraceToken::createToken(std::string str) {
  assert(str == "}");
  return std::unique_ptr<RightBraceToken>(new RightBraceToken());
};

RightBraceToken::RightBraceToken(){};

const std::string RightBraceToken::GetValue() { return "}"; };
}  // namespace token
