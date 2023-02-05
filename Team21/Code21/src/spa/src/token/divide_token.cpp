#include "divide_token.h"

#include <assert.h>

#include <string>

namespace token {
DivideToken::DivideToken(){};

const std::string DivideToken::getValue() { return "/"; };

const DivideToken* DivideToken::createToken(std::string str) {
  assert(str == "/");
  return new DivideToken();
};
}  // namespace token
