#include "assign_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> AssignToken::CreateToken(std::string str) {
  assert(str == "="); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<AssignToken>(new AssignToken());
};

AssignToken::AssignToken(){};

const std::string AssignToken::GetValue() { return "="; };
}  // namespace token
