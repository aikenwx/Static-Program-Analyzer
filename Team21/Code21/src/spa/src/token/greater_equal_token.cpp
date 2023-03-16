#include <assert.h>
#include <string>

#include "greater_equal_token.h"

namespace token {
const std::unique_ptr<Token> GreaterEqualToken::CreateToken(std::string str) {
  assert(str == ">="); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<GreaterEqualToken>(new GreaterEqualToken());
};

GreaterEqualToken::GreaterEqualToken(){};

const std::string GreaterEqualToken::GetValue() { return ">="; };
}  // namespace token
