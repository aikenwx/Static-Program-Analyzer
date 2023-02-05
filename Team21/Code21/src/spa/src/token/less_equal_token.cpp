#include "less_equal_token.h"

#include <assert.h>

#include <string>

namespace token {
LessEqualToken::LessEqualToken(){};

const std::string LessEqualToken::getValue() { return "<="; };

const LessEqualToken* LessEqualToken::createToken(std::string str) {
  assert(str == "<=");
  return new LessEqualToken();
};
}  // namespace token
