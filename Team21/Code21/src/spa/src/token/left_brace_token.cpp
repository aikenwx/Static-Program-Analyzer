#include "left_brace_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> LeftBraceToken::createToken(std::string str) {
  assert(str == "{");
  return std::unique_ptr<LeftBraceToken>(new LeftBraceToken());
};

LeftBraceToken::LeftBraceToken(){};

const std::string LeftBraceToken::getValue() { return "{"; };
}  // namespace token
