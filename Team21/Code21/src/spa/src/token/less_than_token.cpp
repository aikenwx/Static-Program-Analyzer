#include "less_than_token.h"

#include <assert.h>

#include <string>

namespace token {
LessThanToken::LessThanToken(){};

const std::string LessThanToken::getValue() { return "<"; };

const LessThanToken* LessThanToken::createToken(std::string str) {
  assert(str == "<");
  if (instance_ == nullptr) instance_ = new LessThanToken();
  return instance_;
};

LessThanToken* LessThanToken::instance_ = nullptr;
}  // namespace token
