#include <assert.h>
#include <string>

#include "greater_than_token.h"

namespace token {
const std::unique_ptr<Token> GreaterThanToken::CreateToken(std::string str) {
  assert(str == ">"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<GreaterThanToken>(new GreaterThanToken());
};

GreaterThanToken::GreaterThanToken(){};

const std::string GreaterThanToken::GetValue() { return ">"; };
}  // namespace token
