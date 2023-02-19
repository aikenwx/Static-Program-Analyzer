#include "divide_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> DivideToken::createToken(std::string str) {
  assert(str == "/");
  return std::unique_ptr<DivideToken>(new DivideToken());
};

DivideToken::DivideToken(){};

const std::string DivideToken::GetValue() { return "/"; };
}  // namespace token
