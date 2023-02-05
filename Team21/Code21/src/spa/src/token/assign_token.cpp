#include "assign_token.h"

#include <assert.h>

#include <string>

namespace token {
AssignToken::AssignToken(){};

const std::string AssignToken::getValue() { return "="; };

const AssignToken* AssignToken::createToken(std::string str) {
  assert(str == "=");
  return new AssignToken();
};
}  // namespace token