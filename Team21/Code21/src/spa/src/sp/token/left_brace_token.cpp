#include "left_brace_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> LeftBraceToken::CreateToken(std::string str) {
  assert(str == "{"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<LeftBraceToken>(new LeftBraceToken());
};

LeftBraceToken::LeftBraceToken(){};

const std::string LeftBraceToken::GetValue() { return "{"; };
}  // namespace token
