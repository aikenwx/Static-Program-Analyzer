#include "not_token.h"

#include <assert.h>

#include <string>

namespace token {
NotToken::NotToken(){};

const std::string NotToken::getValue() { return "!"; };

const NotToken* NotToken::createToken(std::string str) {
  assert(str == "!");
  if (instance_ == nullptr) instance_ = new NotToken();
  return instance_;
};

NotToken* NotToken::instance_ = nullptr;
}  // namespace token
