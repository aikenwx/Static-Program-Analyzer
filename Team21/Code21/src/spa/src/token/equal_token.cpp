#include "equal_token.h"

#include <assert.h>

#include <string>

namespace token {
EqualToken::EqualToken(){};

const std::string EqualToken::getValue() { return "=="; };

const EqualToken* EqualToken::createToken(std::string str) {
  assert(str == "==");
  if (instance_ == nullptr) instance_ = new EqualToken();
  return instance_;
};

EqualToken* EqualToken::instance_ = nullptr;
}  // namespace token
