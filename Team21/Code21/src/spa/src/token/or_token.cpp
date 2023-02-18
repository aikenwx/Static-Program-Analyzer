#include "or_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> OrToken::createToken(std::string str) {
  assert(str == "||");
  return std::unique_ptr<OrToken>(new OrToken());
};

OrToken::OrToken(){};

const std::string OrToken::getValue() { return "||"; };
}  // namespace token
