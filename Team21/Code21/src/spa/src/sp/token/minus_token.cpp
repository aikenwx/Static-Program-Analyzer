#include <assert.h>
#include <string>

#include "minus_token.h"

namespace token {
const std::unique_ptr<Token> MinusToken::CreateToken(std::string str) {
  assert(str == "-"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<MinusToken>(new MinusToken());
};

MinusToken::MinusToken(){};

const std::string MinusToken::GetValue() { return "-"; };
}  // namespace token
