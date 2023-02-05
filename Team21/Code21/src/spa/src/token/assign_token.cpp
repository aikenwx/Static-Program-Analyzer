#include "assign_token.h"

#include <assert.h>

#include <string>

namespace token {
AssignToken::AssignToken(){};

const std::string AssignToken::getValue() { return "="; };

const AssignToken* AssignToken::createToken(std::string str) {
  assert(str == "=");
  if (instance_ == nullptr) instance_ = new AssignToken();
  return instance_;
};

AssignToken* AssignToken::instance_ = nullptr;
}  // namespace token
