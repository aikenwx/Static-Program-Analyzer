#include "left_paren_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> LeftParenToken::CreateToken(std::string str) {
  assert(str == "("); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<LeftParenToken>(new LeftParenToken());
};

LeftParenToken::LeftParenToken(){};

const std::string LeftParenToken::GetValue() { return "("; };
}  // namespace token
