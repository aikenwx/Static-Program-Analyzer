#include "semicolon_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> SemicolonToken::createToken(
    std::string str) {
  assert(str == ";");
  return std::unique_ptr<SemicolonToken>(new SemicolonToken());
};

SemicolonToken::SemicolonToken(){};

const std::string SemicolonToken::GetValue() { return ";"; };
}  // namespace token
