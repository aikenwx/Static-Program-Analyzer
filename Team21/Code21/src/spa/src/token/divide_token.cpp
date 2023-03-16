#include "divide_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> DivideToken::CreateToken(std::string str) {
  assert(str == "/"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<DivideToken>(new DivideToken());
};

DivideToken::DivideToken(){};

const std::string DivideToken::GetValue() { return "/"; };
}  // namespace token
