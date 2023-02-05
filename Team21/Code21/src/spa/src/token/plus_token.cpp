#include "plus_token.h"

#include <assert.h>

#include <string>

namespace token {
PlusToken::PlusToken(){};

const std::string PlusToken::getValue() { return "+"; };

const PlusToken* PlusToken::createToken(std::string str) {
  assert(str == "+");
  if (instance_ == nullptr) instance_ = new PlusToken();
  return instance_;
};

PlusToken* PlusToken::instance_ = nullptr;
}  // namespace token
