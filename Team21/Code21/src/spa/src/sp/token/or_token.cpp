#include "or_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> OrToken::CreateToken(std::string str) {
  assert(str == "||"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<OrToken>(new OrToken());
};

OrToken::OrToken(){};

const std::string OrToken::GetValue() { return "||"; };
}  // namespace token
