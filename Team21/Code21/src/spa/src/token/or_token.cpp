#include "or_token.h"

#include <assert.h>

#include <string>

namespace token {
OrToken::OrToken(){};

const std::string OrToken::getValue() { return "||"; };

const OrToken* OrToken::createToken(std::string str) {
  assert(str == "||");
  return new OrToken();
};
}  // namespace token