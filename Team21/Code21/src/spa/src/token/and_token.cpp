#include "and_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> AndToken::CreateToken(std::string str) {
  assert(str == "&&");
  return std::unique_ptr<AndToken>(new AndToken());
};

AndToken::AndToken(){};

const std::string AndToken::GetValue() { return "&&"; };
}  // namespace token
