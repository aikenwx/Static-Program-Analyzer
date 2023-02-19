#include "less_equal_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> LessEqualToken::CreateToken(std::string str) {
  assert(str == "<=");
  return std::unique_ptr<LessEqualToken>(new LessEqualToken());
};

LessEqualToken::LessEqualToken(){};

const std::string LessEqualToken::GetValue() { return "<="; };
}  // namespace token
