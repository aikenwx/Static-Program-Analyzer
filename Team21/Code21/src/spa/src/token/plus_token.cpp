#include "plus_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> PlusToken::CreateToken(std::string str) {
  assert(str == "+"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<PlusToken>(new PlusToken());
};

PlusToken::PlusToken(){};

const std::string PlusToken::GetValue() { return "+"; };
}  // namespace token
