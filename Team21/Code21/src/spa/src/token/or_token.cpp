#include "or_token.h"

#include <assert.h>

#include <string>

namespace token {
OrToken::OrToken(){};

const std::string OrToken::getValue() { return "||"; };

const OrToken* OrToken::createToken(std::string str) {
  assert(str == "||");
  if (instance_ == nullptr) instance_ = new OrToken();
  return instance_;
};

OrToken* OrToken::instance_ = nullptr;
}  // namespace token
