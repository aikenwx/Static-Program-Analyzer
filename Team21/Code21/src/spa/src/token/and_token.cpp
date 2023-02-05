#include "and_token.h"

#include <assert.h>

#include <string>

namespace token {
AndToken::AndToken(){};

const std::string AndToken::getValue() { return "&&"; };

const AndToken* AndToken::createToken(std::string str) {
  assert(str == "&&");
  if (instance_ == nullptr) instance_ = new AndToken();
  return instance_;
};

AndToken* AndToken::instance_ = nullptr;
}  // namespace token
