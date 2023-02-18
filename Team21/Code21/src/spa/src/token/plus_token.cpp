#include "plus_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> PlusToken::createToken(std::string str) {
  assert(str == "+");
  return std::unique_ptr<PlusToken>(new PlusToken());
};

PlusToken::PlusToken(){};

const std::string PlusToken::getValue() { return "+"; };
}  // namespace token
