#include "less_than_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> LessThanToken::CreateToken(std::string str) {
  assert(str == "<"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<LessThanToken>(new LessThanToken());
};

LessThanToken::LessThanToken(){};

const std::string LessThanToken::GetValue() { return "<"; };
}  // namespace token
