#include "assign_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> AssignToken::createToken(std::string str) {
  assert(str == "=");
  return std::unique_ptr<AssignToken>(new AssignToken());
};

AssignToken::AssignToken(){};

const std::string AssignToken::getValue() { return "="; };
}  // namespace token
