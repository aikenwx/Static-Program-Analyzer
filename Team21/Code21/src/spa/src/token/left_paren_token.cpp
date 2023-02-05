#include "left_paren_token.h"

#include <assert.h>

#include <string>

namespace token {
LeftParenToken::LeftParenToken(){};

const std::string LeftParenToken::getValue() { return "("; };

const LeftParenToken* LeftParenToken::createToken(std::string str) {
  assert(str == "(");
  if (instance_ == nullptr) instance_ = new LeftParenToken();
  return instance_;
};

LeftParenToken* LeftParenToken::instance_ = nullptr;
}  // namespace token
