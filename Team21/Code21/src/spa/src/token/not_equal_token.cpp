#include "not_equal_token.h"

#include <assert.h>

#include <string>

namespace token {
NotEqualToken::NotEqualToken(){};

const std::string NotEqualToken::getValue() { return "!="; };

const NotEqualToken* NotEqualToken::createToken(std::string str) {
  assert(str == "!=");
  if (instance_ == nullptr) instance_ = new NotEqualToken();
  return instance_;
};

NotEqualToken* NotEqualToken::instance_ = nullptr;
}  // namespace token
