#include "not_equal_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> NotEqualToken::CreateToken(std::string str) {
  assert(str == "!="); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<NotEqualToken>(new NotEqualToken());
};

NotEqualToken::NotEqualToken(){};

const std::string NotEqualToken::GetValue() { return "!="; };
}  // namespace token
