#include "end_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> EndToken::CreateToken(std::string str) {
  assert(str == "$");
  return std::unique_ptr<EndToken>(new EndToken());
};

EndToken::EndToken(){};

const std::string EndToken::GetValue() { return "$"; };
}  // namespace token