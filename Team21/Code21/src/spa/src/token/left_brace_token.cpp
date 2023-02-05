#include "left_brace_token.h"

#include <assert.h>

#include <string>

namespace token {
LeftBraceToken::LeftBraceToken(){};

const std::string LeftBraceToken::getValue() { return "{"; };

const LeftBraceToken* LeftBraceToken::createToken(std::string str) {
  assert(str == "{");
  if (instance_ == nullptr) instance_ = new LeftBraceToken();
  return instance_;
};

LeftBraceToken* LeftBraceToken::instance_ = nullptr;
}  // namespace token
