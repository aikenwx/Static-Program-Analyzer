#include "left_brace_token.h"

#include <assert.h>

#include <string>

namespace token {
LeftBraceToken::LeftBraceToken(){};

const std::string LeftBraceToken::getValue() { return "{"; };

const LeftBraceToken* LeftBraceToken::createToken(std::string str) {
  assert(str == "{");
  return new LeftBraceToken();
}
}  // namespace token