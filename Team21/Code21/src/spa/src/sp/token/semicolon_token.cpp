#include "semicolon_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> SemicolonToken::CreateToken(
    std::string str) {
  assert(str == ";"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<SemicolonToken>(new SemicolonToken());
};

SemicolonToken::SemicolonToken(){};

const std::string SemicolonToken::GetValue() { return ";"; };
}  // namespace token
