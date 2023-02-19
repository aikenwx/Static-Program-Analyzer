#include "left_paren_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> LeftParenToken::createToken(std::string str) {
  assert(str == "(");
  return std::unique_ptr<LeftParenToken>(new LeftParenToken());
};

LeftParenToken::LeftParenToken(){};

const std::string LeftParenToken::GetValue() { return "("; };
}  // namespace token
