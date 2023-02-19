#include <assert.h>
#include <string>

#include "greater_equal_token.h"

namespace token {
const std::unique_ptr<Token> GreaterEqualToken::CreateToken(std::string str) {
  assert(str == ">=");
  return std::unique_ptr<GreaterEqualToken>(new GreaterEqualToken());
};

GreaterEqualToken::GreaterEqualToken(){};

const std::string GreaterEqualToken::GetValue() { return ">="; };
}  // namespace token
