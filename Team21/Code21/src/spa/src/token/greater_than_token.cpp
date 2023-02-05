#include <assert.h>
#include <string>

#include "greater_than_token.h"

namespace token {
GreaterThanToken::GreaterThanToken(){};

const std::string GreaterThanToken::getValue() { return ">"; };

const GreaterThanToken* GreaterThanToken::createToken(std::string str) {
  assert(str == ">");
  if (instance_ == nullptr) instance_ = new GreaterThanToken();
  return instance_;
};

GreaterThanToken* GreaterThanToken::instance_ = nullptr;
}  // namespace token
