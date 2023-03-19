#include "right_brace_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> RightBraceToken::CreateToken(std::string str) {
  assert(str == "}"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<RightBraceToken>(new RightBraceToken());
};

RightBraceToken::RightBraceToken(){};

const std::string RightBraceToken::GetValue() { return "}"; };
}  // namespace token
