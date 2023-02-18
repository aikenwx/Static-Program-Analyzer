#include "not_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> NotToken::createToken(std::string str) {
  assert(str == "!");
  return std::unique_ptr<NotToken>(new NotToken());
};

NotToken::NotToken(){};

const std::string NotToken::getValue() { return "!"; };
}  // namespace token
