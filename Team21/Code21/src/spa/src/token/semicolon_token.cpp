#include "semicolon_token.h"

#include <assert.h>

#include <string>

namespace token {
SemicolonToken::SemicolonToken(){};

const std::string SemicolonToken::getValue() { return ";"; };

const SemicolonToken* SemicolonToken::createToken(std::string str) {
  assert(str == ";");
  if (instance_ == nullptr) instance_ = new SemicolonToken();
  return instance_;
};

SemicolonToken* SemicolonToken::instance_ = nullptr;
}  // namespace token