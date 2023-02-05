#include "right_brace_token.h"

#include <assert.h>

#include <string>

namespace token {
RightBraceToken::RightBraceToken(){};

const std::string RightBraceToken::getValue() { return "}"; };

const RightBraceToken* RightBraceToken::createToken(std::string str) {
  assert(str == "}");
  if (instance_ == nullptr) instance_ = new RightBraceToken();
  return instance_;
};

RightBraceToken* RightBraceToken::instance_ = nullptr;
}  // namespace token
