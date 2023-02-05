#include "right_paren_token.h"

#include <assert.h>

#include <string>

namespace token {
RightParenToken::RightParenToken(){};

const std::string RightParenToken::getValue() { return ")"; };

const RightParenToken* RightParenToken::createToken(std::string str) {
  assert(str == ")");
  if (instance_ == nullptr) instance_ = new RightParenToken();
  return instance_;
};

RightParenToken* RightParenToken::instance_ = nullptr;
}  // namespace token
