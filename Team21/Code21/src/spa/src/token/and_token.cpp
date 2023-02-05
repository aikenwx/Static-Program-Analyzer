#include "and_token.h"

#include <assert.h>

#include <string>

namespace token {
AndToken::AndToken(){};

const std::string AndToken::getValue() { return "&&"; };

const AndToken* AndToken::createToken(std::string str) {
  assert(str == "&&");
  return new AndToken();
};
}  // namespace token
