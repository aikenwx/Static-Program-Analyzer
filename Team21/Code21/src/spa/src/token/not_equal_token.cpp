#include "not_equal_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> NotEqualToken::createToken(std::string str) {
  assert(str == "!=");
  return std::unique_ptr<NotEqualToken>(new NotEqualToken());
};

NotEqualToken::NotEqualToken(){};

const std::string NotEqualToken::GetValue() { return "!="; };
}  // namespace token
