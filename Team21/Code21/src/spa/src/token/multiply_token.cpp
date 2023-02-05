#include "multiply_token.h"

#include <assert.h>

#include <string>

namespace token {
MultiplyToken::MultiplyToken(){};

const std::string MultiplyToken::getValue() { return "*"; };

const MultiplyToken* MultiplyToken::createToken(std::string str) {
  assert(str == "*");
  if (instance_ == nullptr) instance_ = new MultiplyToken();
  return instance_;
};

MultiplyToken* MultiplyToken::instance_ = nullptr;
}  // namespace token
