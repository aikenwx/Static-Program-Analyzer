#include <assert.h>
#include <string>

#include "plus_token.h"

namespace token {
PlusToken::PlusToken() {}

const std::string PlusToken::getValue() { return "+"; }

const PlusToken* PlusToken::createToken(std::string str) {
  assert(str == "+");
  return new PlusToken();
}
}  // namespace token
