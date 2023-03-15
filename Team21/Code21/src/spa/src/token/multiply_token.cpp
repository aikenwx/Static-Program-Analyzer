#include "multiply_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> MultiplyToken::CreateToken(std::string str) {
  assert(str == "*"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<MultiplyToken>(new MultiplyToken());
};

MultiplyToken::MultiplyToken(){};

const std::string MultiplyToken::GetValue() { return "*"; };
}  // namespace token
